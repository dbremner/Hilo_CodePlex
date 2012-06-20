//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "DelegateCommand.h"
#include "CropImageViewModel.h"
#include "TaskExceptionsExtensions.h"
#include "AsyncException.h"
#include "IPhoto.h"
#include "IRepository.h"
#include "ImageNavigationData.h"
#include "NullPhotoGroup.h"
#include "SimpleQueryOperation.h"
#include <robuffer.h>

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace std;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Imaging;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;

CropImageViewModel::CropImageViewModel(IRepository^ repository, IExceptionPolicy^ exceptionPolicy) : ImageBase(exceptionPolicy), m_repository(repository)
{
    m_saveCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &CropImageViewModel::SaveImage), nullptr);
    m_cancelCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &CropImageViewModel::CancelCrop), nullptr);

    ViewModelBase::m_isAppBarSticky = true;
}

ICommand^ CropImageViewModel::SaveCommand::get()
{
    return m_saveCommand;
}

ICommand^ CropImageViewModel::CancelCommand::get()
{
    return m_cancelCommand;
}

bool CropImageViewModel::InProgress::get()
{
    return m_inProgress;
}

double CropImageViewModel::CropOverlayLeft::get()
{
    return m_cropOverlayLeft;
}

double CropImageViewModel::CropOverlayTop::get()
{
    return m_cropOverlayTop;
}

double CropImageViewModel::CropOverlayHeight::get()
{
    return m_cropOverlayHeight;
}

double CropImageViewModel::CropOverlayWidth::get()
{
    return m_cropOverlayWidth;
}

bool CropImageViewModel::IsCropOverlayVisible::get()
{
    return m_isCropOverlayVisible;
}

ImageSource^ CropImageViewModel::Image::get()
{
    return m_image;
}

void CropImageViewModel::OnNavigatedTo(NavigationEventArgs^ e)
{
    String^ data = dynamic_cast<String^>(e->Parameter); 
    ImageNavigationData^ navigationData = ref new ImageNavigationData(data);

    Initialize(navigationData);
}

task<void> CropImageViewModel::GetImageAsync()
{
    return create_task(m_photo->OpenReadAsync())
        .then([this](IRandomAccessStreamWithContentType^ imageData)
    {
        m_imageStream = imageData;
        return BitmapDecoder::CreateAsync(m_imageStream);
    }).then([this](BitmapDecoder^ decoder)
    {
        m_image = ref new WriteableBitmap(decoder->PixelWidth, decoder->PixelHeight);
        m_imageStream->Seek(0);
        m_image->SetSource(m_imageStream);
        OnPropertyChanged("Image");
    });
}

void CropImageViewModel::Initialize(ImageNavigationData^ navigationData)
{
    auto t = create_task(m_repository->GetPhotoForGroupWithQueryOperationAsync(ref new NullPhotoGroup(), ref new SimpleQueryOperation(navigationData->FilePath)));
    t.then([this](IPhoto^ photo)
    {
        m_photo = nullptr;
        m_image = nullptr;

        m_photo = photo;
        return GetImageAsync();
    }).then(ObserveException<void>(m_exceptionPolicy));
}

byte* CropImageViewModel::GetPointerToPixelData(IBuffer^ buffer)
{
    IUnknown* pUnknown = reinterpret_cast<IUnknown*>(buffer);
    IBufferByteAccess* pBufferByteAccess = nullptr;
    HRESULT hr = pUnknown->QueryInterface(IID_PPV_ARGS(&pBufferByteAccess));
    pUnknown->Release();
    byte *pPixels = nullptr;
    hr = pBufferByteAccess->Buffer(&pPixels);
    return pPixels;
}

task<void> CropImageViewModel::EncodeImageAsync()
{
    auto continuationContext = task_continuation_context::use_arbitrary();
    auto ras = ref new InMemoryRandomAccessStream();
    auto bitmapEncoder = std::make_shared<BitmapEncoder^>(nullptr);
    unsigned int width = m_image->PixelWidth;
    unsigned int height = m_image->PixelHeight;

    return task<BitmapDecoder^>(BitmapDecoder::CreateAsync(m_imageStream))
        .then([ras](BitmapDecoder^ decoder)
    {
        return BitmapEncoder::CreateForTranscodingAsync(ras, decoder);
    }, continuationContext).then([this, width, height, bitmapEncoder](BitmapEncoder^ encoder)
    {
        BitmapBounds bounds;
        bounds.X = m_cropX;
        bounds.Y = m_cropY;
        bounds.Width = width;
        bounds.Height = height;
        encoder->BitmapTransform->Bounds = bounds;

        // Force a new thumbnail to reflect any rotation operation
        encoder->IsThumbnailGenerated = true;
        (*bitmapEncoder) = encoder;
        return encoder->FlushAsync();
    }, continuationContext).then([this, ras, bitmapEncoder](task<void> encodeTask)
    {
        try
        {
            encodeTask.get();
        }
        catch (Exception^ ex)
        {
            switch (ex->HResult)
            {
            case WINCODEC_ERR_UNSUPPORTEDOPERATION:
                // If the encoder does not support writing a thumbnail, then
                // disable thumbnail generation before trying again.
                (*bitmapEncoder)->IsThumbnailGenerated = false;
                break;
            default:
                throw ex;
            }
        }
        if ((*bitmapEncoder)->IsThumbnailGenerated == false)
        {
            return (*bitmapEncoder)->FlushAsync();
        }
        IAsyncAction^ action = create_async([]{});
        return action;
    }, continuationContext).then([this, ras]()
    {
        m_imageStream = ras;
    });
}

void CropImageViewModel::ChangeInProgress(bool value)
{
    m_inProgress = value;
    OnPropertyChanged("InProgress");
}

void CropImageViewModel::SaveImage(Object^ parameter)
{
    if (m_isSaving) return;

    m_isSaving = true;
    auto file = std::make_shared<StorageFile^>(nullptr);

    task<StorageFile^> saveTask = create_task(ImageBase::GetFileNameFromFileSavePickerAsync(m_photo->FileType));
    saveTask.then([this, file](StorageFile^ f)
    {
        ChangeInProgress(true);
        (*file) = f;
        return EncodeImageAsync();
    }).then([this, file]()
    {
        return ImageBase::SaveImageAsync((*file), m_imageStream);
    }).then([this](task<void> priorTask)
    {
        m_isSaving = false;
        ChangeInProgress(false);
        try
        {
            priorTask.get();
        }
        catch(const Concurrency::task_canceled&)
        {
        }
    }).then(ObserveException<void>(m_exceptionPolicy));
}

void CropImageViewModel::CancelCrop(Object^ parameter)
{
    ViewModelBase::GoBack();
}

void CropImageViewModel::CalculateInitialCropOverlayPosition(GeneralTransform^ transform, float width, float height)
{
    // Find the onscreen coordinates of the top left corner of the photo.
    Point topLeft;
    topLeft.X = 0;
    topLeft.Y = 0;
    if (transform != nullptr)
    {
        topLeft = transform->TransformPoint(topLeft);
    }

    m_left = topLeft.X;
    m_top = topLeft.Y;
    m_bottom = m_top + height;
    m_right = m_left + width;
    m_cropOverlayTop = m_top;
    m_cropOverlayLeft = m_left;
    m_cropOverlayHeight = height;
    m_cropOverlayWidth = width;
    m_actualHeight = height;
    m_actualWidth = width;
    m_isCropOverlayVisible = true;

    OnPropertyChanged("CropOverlayLeft");
    OnPropertyChanged("CropOverlayTop");
    OnPropertyChanged("CropOverlayHeight");
    OnPropertyChanged("CropOverlayWidth");
    OnPropertyChanged("IsCropOverlayVisible");
}

void CropImageViewModel::UpdateCropOverlayPostion(Thumb ^thumb, double verticalChange, double horizontalChange, double minWidth, double minHeight)
{
    if (thumb != nullptr)
    {
        double deltaH, deltaV, left, top;

        switch (thumb->VerticalAlignment)
        {
        case VerticalAlignment::Bottom:
            deltaV = min(-verticalChange, m_cropOverlayHeight - minHeight);
            m_cropOverlayHeight -= deltaV;
            if (m_cropOverlayHeight + m_cropOverlayTop > m_bottom)
            {
                m_cropOverlayHeight = m_bottom - m_cropOverlayTop;
            }
            OnPropertyChanged("CropOverlayHeight");
            break;
        case VerticalAlignment::Top:
            deltaV = min(verticalChange, m_cropOverlayHeight - minHeight);
            top = m_cropOverlayTop + deltaV;
            if (m_top < top)
            {
                m_cropOverlayTop = top;
                m_cropOverlayHeight -= deltaV;
                if (m_cropOverlayHeight > m_actualHeight)
                {
                    m_cropOverlayHeight = m_actualHeight;
                }
                OnPropertyChanged("CropOverlayTop");
                OnPropertyChanged("CropOverlayHeight");
            }
            break;
        default:
            break;
        }

        switch (thumb->HorizontalAlignment)
        {
        case HorizontalAlignment::Left:
            deltaH = min(horizontalChange, m_cropOverlayWidth - minWidth);
            left = m_cropOverlayLeft + deltaH;
            if (m_left < left)
            {
                m_cropOverlayLeft = left;
                m_cropOverlayWidth -= deltaH;
                if (m_cropOverlayWidth > m_actualWidth)
                {
                    m_cropOverlayWidth = m_actualWidth;
                }
                OnPropertyChanged("CropOverlayLeft");
                OnPropertyChanged("CropOverlayWidth");
            }
            break;
        case HorizontalAlignment::Right:
            deltaH = min(-horizontalChange, m_cropOverlayWidth - minWidth);
            m_cropOverlayWidth -= deltaH;
            if (m_cropOverlayWidth + m_cropOverlayLeft > m_right)
            {
                m_cropOverlayWidth = m_right - m_cropOverlayLeft;
            }
            OnPropertyChanged("CropOverlayWidth");
            break;

        default:
            break;
        }
    }
}

IAsyncAction^ CropImageViewModel::CropImageAsync(double actualWidth)
{
    return concurrency::create_async([this, actualWidth]
    { 
        return CropImageAsyncImpl(actualWidth).then(ObserveException<void>(m_exceptionPolicy)); 
    });
}

task<void> CropImageViewModel::CropImageAsyncImpl(double actualWidth)
{
    m_inProgress = true;
    OnPropertyChanged("InProgress");

    auto cropTask = create_task([]{});
    return cropTask.then([this, actualWidth]()
    {
        // Calculate crop values
        double scaleFactor = m_image->PixelWidth / actualWidth;
        unsigned int scaledXStart = safe_cast<int>((m_cropOverlayLeft - m_left) * scaleFactor);
        unsigned int scaledYStart = safe_cast<int>((m_cropOverlayTop - m_top) * scaleFactor);
        unsigned int j = scaledYStart;
        unsigned int i = scaledXStart;
        unsigned int newWidth = safe_cast<unsigned int>(m_cropOverlayWidth * scaleFactor); 
        unsigned int newHeight = safe_cast<unsigned int>(m_cropOverlayHeight * scaleFactor);

        m_cropX += scaledXStart;
        m_cropY += scaledYStart;

        // Create destination bitmap
        WriteableBitmap^ destImage = ref new WriteableBitmap(newWidth, newHeight);

        // Get pointers to the source and destination pixel data
        byte* pSrcPixels = GetPointerToPixelData(m_image->PixelBuffer);
        byte* pDestPixels = GetPointerToPixelData(destImage->PixelBuffer);        

        // Crop the image
        for (unsigned int y = 0; y < newHeight; y++)
        {
            for (unsigned int x = 0; x < newWidth; x++)
            {
                pDestPixels[(x + y * newWidth) * 4] = 
                    pSrcPixels[(i + j * m_image->PixelWidth) * 4];     // B
                pDestPixels[(x + y * newWidth) * 4 + 1] = 
                    pSrcPixels[(i + j * m_image->PixelWidth) * 4 + 1]; // G
                pDestPixels[(x + y * newWidth) * 4 + 2] = 
                    pSrcPixels[(i + j * m_image->PixelWidth) * 4 + 2]; // R
                pDestPixels[(x + y * newWidth) * 4 + 3] =
                    pSrcPixels[(i + j * m_image->PixelWidth) * 4 + 3]; // A
                i++;
                if (i >= (newWidth + scaledXStart))
                {
                    i = scaledXStart;
                }
            }
            j++;
        }

        // Update image on screen
        m_image = destImage;
        m_inProgress = false;
        OnPropertyChanged("InProgress");
        OnPropertyChanged("Image");
    }, task_continuation_context::use_current());
}

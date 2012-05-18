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

using namespace Hilo;

using namespace concurrency;
using namespace std;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Imaging;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Streams;

CropImageViewModel::CropImageViewModel(IExceptionPolicy^ exceptionPolicy) : ImageBase(exceptionPolicy)
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

Object^ CropImageViewModel::FileName::get()
{
    return m_file->Name;
}

Object^ CropImageViewModel::FileDateCreated::get()
{
    return ref new Box<DateTime>(m_file->DateCreated);
}

Object^ CropImageViewModel::FileDateModified::get()
{
    return ref new Box<DateTime>(m_file->BasicProperties->DateModified);
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

ImageSource^ CropImageViewModel::Photo::get()
{
    if (m_image == nullptr && m_file != nullptr)
    {
        auto fileOpenTask = task<IRandomAccessStreamWithContentType^>(m_file->OpenReadAsync());
        fileOpenTask.then([this](IRandomAccessStreamWithContentType^ imageData)
        {
            m_imageStream = imageData;
            m_image = ref new BitmapImage();
            m_image->SetSource(m_imageStream);
            OnPropertyChanged("Photo");
        }, task_continuation_context::use_current());
    }
    return m_image;
}

void CropImageViewModel::OnNavigatedTo(NavigationEventArgs^ e)
{
    m_file = dynamic_cast<FileInformation^>(e->Parameter);
    m_image = nullptr;
}

void CropImageViewModel::SaveImage(Object^ parameter)
{
    AsyncException::ObserveWithPolicy(m_exceptionPolicy, ImageBase::SaveImageAsync(m_file, m_imageStream));
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
    auto continuationContext = concurrency::task_continuation_context::use_current();

    auto decoder = make_shared<BitmapDecoder^>(nullptr);
    auto pixelProvider = make_shared<PixelDataProvider^>(nullptr);
    auto newWidth = make_shared<unsigned int>(0);
    auto newHeight = make_shared<unsigned int>(0);
    auto bitmapStream = ref new InMemoryRandomAccessStream();

    auto cropImageTask = task<BitmapDecoder^>(BitmapDecoder::CreateAsync(m_imageStream));
    return cropImageTask.then([decoder](BitmapDecoder^ createdDecoder)
    {
        (*decoder) = createdDecoder;
        return (*decoder)->GetPixelDataAsync(BitmapPixelFormat::Rgba8,
            BitmapAlphaMode::Straight,
            ref new BitmapTransform(),
            ExifOrientationMode::RespectExifOrientation,
            ColorManagementMode::ColorManageToSRgb);
    }).then([this, decoder, bitmapStream, pixelProvider](PixelDataProvider^ provider)
    {
        (*pixelProvider) = provider;
        return BitmapEncoder::CreateAsync(BitmapEncoder::JpegEncoderId, bitmapStream);
        //return BitmapEncoder::CreateForTranscodingAsync(bitmapStream, (*decoder));
    }).then([this, pixelProvider, decoder, actualWidth, newWidth, newHeight](BitmapEncoder ^encoder)
    {
        Array<unsigned char, 1>^ sourcePixels = (*pixelProvider)->DetachPixelData();
        double scaleFactor = (*decoder)->PixelWidth / actualWidth;
        unsigned int scaledXStart = safe_cast<int>((m_cropOverlayLeft - m_left) * scaleFactor);
        unsigned int scaledYStart = safe_cast<int>((m_cropOverlayTop - m_top) * scaleFactor);
        unsigned int j = scaledYStart;
        unsigned int i = scaledXStart;

        *newWidth = safe_cast<int>(m_cropOverlayWidth * scaleFactor);
        *newHeight = safe_cast<int>(m_cropOverlayHeight * scaleFactor);
        auto destinationPixels = ref new Array<unsigned char>((*newWidth * *newHeight) * 4);

        for (unsigned int y = 0; y < *newHeight; y++)
        {
            for (unsigned int x = 0; x < *newWidth; x++)
            {
                destinationPixels[(x + y * *newWidth) * 4]     = 
                    sourcePixels[(i + j * (*decoder)->OrientedPixelWidth) * 4];
                destinationPixels[(x + y * *newWidth) * 4 + 1] = 
                    sourcePixels[(i + j * (*decoder)->OrientedPixelWidth) * 4 + 1];
                destinationPixels[(x + y * *newWidth) * 4 + 2] = 
                    sourcePixels[(i + j * (*decoder)->OrientedPixelWidth) * 4 + 2];
                destinationPixels[(x + y * *newWidth) * 4 + 3] = 
                    sourcePixels[(i + j * (*decoder)->OrientedPixelWidth) * 4 + 3];
                i++;
                if (i >= (*newWidth + scaledXStart))
                {
                    i = scaledXStart;
                }
            }
            j++;
        }

        encoder->SetPixelData(BitmapPixelFormat::Rgba8,
            BitmapAlphaMode::Straight,
            *newWidth,
            *newHeight,
            (*decoder)->DpiX,
            (*decoder)->DpiY,
            destinationPixels);
        sourcePixels = nullptr;
        return encoder->FlushAsync();
    }).then([this, bitmapStream]()
    {
        m_imageStream = bitmapStream;
        m_image->SetSource(m_imageStream);
        m_inProgress = false;
        OnPropertyChanged("InProgress");
        OnPropertyChanged("Photo");
    }, continuationContext);
}

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
#include "PhotoReader.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Imaging;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Streams;

CropImageViewModel::CropImageViewModel(void)
{
    m_saveCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &CropImageViewModel::SaveImage), nullptr);
    m_cancelCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &CropImageViewModel::CancelCrop), nullptr);
    
    ViewModelBase::m_isAppBarSticky = true;
}

CropImageViewModel::~CropImageViewModel(void)
{
    // Deallocate pixel arrays
    if (m_sourcePixels != nullptr)
    {
        m_sourcePixels = nullptr;
    }

    if (m_destinationPixels != nullptr)
    {
        m_destinationPixels = nullptr;
    }
}

int CropImageViewModel::CropRectangleWidth::get()
{
    return m_cropRectangleWidth;
}

int CropImageViewModel::CropRectangleHeight::get()
{
    return m_cropRectangleHeight;
}

Object^ CropImageViewModel::CropRectangleMargin::get()
{
    return m_cropRectangleMargin;
}

bool CropImageViewModel::IsCropRectangleVisible::get()
{
    return m_isCropRectangleVisible;
}

ICommand^ CropImageViewModel::SaveCommand::get()
{
    return m_saveCommand;
}

ICommand^ CropImageViewModel::CancelCommand::get()
{
    return m_cancelCommand;
}

ImageSource^ CropImageViewModel::Photo::get()
{
    if (m_image == nullptr && m_file != nullptr)
    {
        auto fileOpenTask = task<IRandomAccessStreamWithContentType^>(m_file->OpenReadAsync());
        fileOpenTask.then([this](IRandomAccessStreamWithContentType^ imageData)
        {
            m_randomAccessStream = imageData;
            m_image = ref new BitmapImage();
            m_image->SetSource(m_randomAccessStream);
        }).then([this]
        {
            OnPropertyChanged("Photo");
            PopulatePixelArray();
        }, task_continuation_context::use_current());
    }
    return m_image;
}

void CropImageViewModel::OnNavigatedTo(NavigationEventArgs^ e)
{
    m_file = dynamic_cast<FileInformation^>(e->Parameter);
    m_image = nullptr;
}

void CropImageViewModel::PopulatePixelArray()
{
    task<BitmapDecoder^> decodeTask(BitmapDecoder::CreateAsync(m_randomAccessStream));
    decodeTask.then([this](task<BitmapDecoder^> bmpDecodertask) {
        BitmapDecoder^ bmpDecoder;
        try {
            bmpDecoder = bmpDecodertask.get();
            m_decoder = bmpDecoder;
            m_imageWidth = bmpDecoder->PixelWidth;
            m_imageHeight = bmpDecoder->PixelHeight;
        }
        catch (Platform::Exception^ ex)
        {
            String^ s = ex->Message;
        }
        return bmpDecoder->GetPixelDataAsync(BitmapPixelFormat::Rgba8, BitmapAlphaMode::Straight, ref new BitmapTransform(), 
            ExifOrientationMode::RespectExifOrientation, ColorManagementMode::ColorManageToSRgb);
    }).then([this](PixelDataProvider^ provider) {
        m_sourcePixels = provider->DetachPixelData();
    });
}

void CropImageViewModel::SaveImage(Object^ parameter)
{
    ImageBase::SaveImage(m_file, m_randomAccessStream);
}

void CropImageViewModel::CancelCrop(Object^ parameter)
{
    ViewModelBase::GoBack();
}

void CropImageViewModel::GetCropStartCoordinates(Point onScreen, Point relative, int pageTitleRowHeight)
{
    m_pageTitleRowHeight = pageTitleRowHeight;

    // On-screen coordinates
    m_actualCropAnchorPoint.X = onScreen.X;
    m_actualCropAnchorPoint.Y = onScreen.Y - m_pageTitleRowHeight;

    // Image coordinates
    m_relativeCropAnchorPoint.X = relative.X;
    m_relativeCropAnchorPoint.Y = relative.Y;

    m_cropRectangleHeight = 0;
    m_cropRectangleWidth = 0;
}

void CropImageViewModel::CalculateCropRectangleCoordinates(Point onScreen, Point relative, Thickness imageMargin)
{
    // On-screen coordinates
	m_actualCropEndPoint = onScreen;
	m_relativeCropEndPoint = relative;
    float x1 = onScreen.X;
    float y1 = onScreen.Y - m_pageTitleRowHeight;
    float x2 = m_actualCropAnchorPoint.X;
    float y2 = m_actualCropAnchorPoint.Y;

    auto margin = imageMargin;
    margin.Left += x1 < x2 ? x1 : x2;
    margin.Top  += y1 < y2 ? y1 : y2;
    m_cropRectangleMargin = margin;
    m_cropRectangleWidth = safe_cast<int>(abs(x1 - x2));
    m_cropRectangleHeight = safe_cast<int>(abs(y1 - y2));
    m_isCropRectangleVisible = true;

    OnPropertyChanged("CropRectangleMargin");
    OnPropertyChanged("CropRectangleWidth");
    OnPropertyChanged("CropRectangleHeight");
    OnPropertyChanged("IsCropRectangleVisible");
}

void CropImageViewModel::CropImage(double actualWidth)
{
    m_isCropRectangleVisible = false;
    OnPropertyChanged("IsCropRectangleVisible");

    double scaleFactor = m_decoder->PixelWidth / actualWidth;

	unsigned int smallestX = safe_cast<int>(min(m_relativeCropAnchorPoint.X,m_relativeCropEndPoint.X));
	unsigned int smallestY = safe_cast<int>(min(m_relativeCropAnchorPoint.Y,m_relativeCropEndPoint.Y));

    unsigned int scaledWidth = safe_cast<int>(m_cropRectangleWidth * scaleFactor);
    unsigned int scaledHeight = safe_cast<int>(m_cropRectangleHeight * scaleFactor);
    unsigned int scaledXStart = safe_cast<int>(smallestX * scaleFactor);
    unsigned int scaledYStart = safe_cast<int>(smallestY * scaleFactor);
    unsigned int j = scaledYStart;
    unsigned int i = scaledXStart;

    m_destinationPixels = ref new Array<unsigned char>((scaledWidth * scaledHeight) * 4);

    for (unsigned int y = 0; y < scaledHeight; y++)
    {
        for (unsigned int x = 0; x < scaledWidth; x++)
        {
            m_destinationPixels[(x + y * scaledWidth) * 4]     = m_sourcePixels[(i + j * m_imageWidth) * 4];
            m_destinationPixels[(x + y * scaledWidth) * 4 + 1] = m_sourcePixels[(i + j * m_imageWidth) * 4 + 1];
            m_destinationPixels[(x + y * scaledWidth) * 4 + 2] = m_sourcePixels[(i + j * m_imageWidth) * 4 + 2];
            m_destinationPixels[(x + y * scaledWidth) * 4 + 3] = m_sourcePixels[(i + j * m_imageWidth) * 4 + 3];
            i++;
            if (i >= (scaledWidth + scaledXStart))
            {
                i = scaledXStart;
            }
        }
        j++;
    }

    m_newImageWidth = scaledWidth;
    m_newImageHeight = scaledHeight;

    auto bitmapStream = ref new InMemoryRandomAccessStream();
    auto encodeTask = task<BitmapEncoder^>(BitmapEncoder::CreateAsync(BitmapEncoder::JpegEncoderId, bitmapStream));
    //auto encodeTask = task<BitmapEncoder^>(BitmapEncoder::CreateForTranscodingAsync(bitmapStream, m_decoder));
    encodeTask.then([this](BitmapEncoder^ encoder) 
    {
        encoder->SetPixelData(BitmapPixelFormat::Rgba8, BitmapAlphaMode::Straight, m_newImageWidth, m_newImageHeight, m_decoder->DpiX, m_decoder->DpiY, m_destinationPixels);
        return encoder->FlushAsync();
    }).then([this, bitmapStream]() {
        m_randomAccessStream = bitmapStream;
        m_image->SetSource(m_randomAccessStream);
    }).then([this]() {
        OnPropertyChanged("Photo");
        PopulatePixelArray();
    });
}

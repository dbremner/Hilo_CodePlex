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
#include "ImageRotation.h"
#include "EditImageViewModel.h"
#include "PhotoReader.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Streams;
using namespace Windows::Storage::Pickers;
using namespace Windows::Graphics::Imaging;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;

EditImageViewModel::EditImageViewModel() : m_angle(0.0)
{
    RotateCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &EditImageViewModel::Rotate90), nullptr);
    UndoEditCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &EditImageViewModel::Undo), nullptr);
    CancelEditCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &EditImageViewModel::Cancel), nullptr);
    SaveEditCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &EditImageViewModel::SaveImage), nullptr);    
}

// TODO: Ensure destructor is invoked
EditImageViewModel::~EditImageViewModel(void)
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

// Properties
ImageSource^ EditImageViewModel::Photo::get()
{
    if (m_image == nullptr && m_file != nullptr)
    {
        auto fileOpenTask = task<IRandomAccessStreamWithContentType^>(m_file->OpenReadAsync());
        fileOpenTask.then([this](IRandomAccessStreamWithContentType^ imageData) {
            m_sourceImageStream = imageData;
            m_image = ref new BitmapImage();
            m_image->SetSource(m_sourceImageStream);
        }).then([this] {
            OnPropertyChanged("Photo");
            PopulatePixelArray(m_sourceImageStream);
        },task_continuation_context::use_current());
    }
    return m_image;
}

void EditImageViewModel::OnNavigatedTo(NavigationEventArgs^ e)
{
    m_file = dynamic_cast<FileInformation^>(e->Parameter);
    m_image = nullptr;
}

bool EditImageViewModel::IsCropRectangleVisible::get()
{
    return m_isCropRectangleVisible;
}

int EditImageViewModel::CropRectangleWidth::get()
{
    return m_cropRectangleWidth;
}

int EditImageViewModel::CropRectangleHeight::get()
{
    return m_cropRectangleHeight;
}

Object^ EditImageViewModel::CropRectangleMargin::get()
{
    return m_cropRectangleMargin;
}

double EditImageViewModel::Angle::get()
{
    return m_angle;
}

void EditImageViewModel::PopulatePixelArray(IRandomAccessStream^ stream)
{

    task<BitmapDecoder^> decodeTask(BitmapDecoder::CreateAsync(stream));
    //    decodeTask.then([this](BitmapDecoder^ bmpDecoder) {
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

void EditImageViewModel::RotateImage()
{
    auto size = ImageRotation::CalculateRotatedImageSize(m_angle, m_imageWidth, m_imageHeight);
    m_newImageWidth = safe_cast<int>(size.Width);
    m_newImageHeight = safe_cast<int>(size.Height);
    m_destinationPixels = ImageRotation::Rotate(m_sourcePixels, m_angle, m_imageWidth, m_imageHeight, m_newImageWidth, m_newImageHeight);
}

// Private member functions that implement Commands
void EditImageViewModel::Rotate90(Object^ parameter)
{   
    auto ras = ref new InMemoryRandomAccessStream();
    task<BitmapEncoder^> encoderTask(BitmapEncoder::CreateForTranscodingAsync(ras, m_decoder));
    encoderTask.then([](BitmapEncoder^ encoder)  {
        encoder->BitmapTransform->Rotation = BitmapRotation::Clockwise90Degrees;
        return encoder->FlushAsync();
    }).then([this,ras](task<void> t){
        t.get();
        m_image->SetSource(ras);
    }).then([this, ras]() {
        PopulatePixelArray(ras);
    }).then([this](){
        OnPropertyChanged("Photo");
    });

}


void EditImageViewModel::Undo(Object^ parameter)
{
}

void EditImageViewModel::Cancel(Object^ parameter)
{
    ViewModelBase::GoBack();
}

void EditImageViewModel::SaveImage(Object^ parameter)
{
    //FileRandomAccessStream^ outputstream;
    auto savePicker = ref new FileSavePicker();
    savePicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;

    auto fileExtension = ref new Vector<String^>();
    fileExtension->Append(m_file->FileType);
    savePicker->FileTypeChoices->Insert("Image", fileExtension);
    savePicker->DefaultFileExtension = m_file->FileType;
    std::shared_ptr<IBuffer^> buffer = std::make_shared<IBuffer^>(nullptr);
    std::shared_ptr<String^> newFileName = std::make_shared<String^>(nullptr);
    auto streamReader = ref new DataReader(m_sourceImageStream);
    concurrency::task<unsigned int> loadStreamTask(streamReader->LoadAsync(static_cast<unsigned int>(m_sourceImageStream->Size)));
    loadStreamTask.then([this,streamReader,buffer,savePicker](unsigned int loadedBytes){
        *buffer = streamReader->ReadBuffer(streamReader->UnconsumedBufferLength);
        return savePicker->PickSaveFileAsync();
    }).then([this, newFileName](StorageFile^ file) {
        //m_file=file;
        *newFileName = file->Path;
        if (file == nullptr)
        {
            cancel_current_task();
        }
        return file;
    }).then([buffer](StorageFile^ file){
        return FileIO::WriteBufferAsync(file, *buffer);
    }).then([this, newFileName]() {
        PhotoReader reader;
        return reader.GetPhotoGroupAsync(*newFileName, 1);
    }).then([this] (IVectorView<FileInformation^>^ files) {
        if (files->Size > 0)
        {
            m_file = files->GetAt(0);
        }
        ViewModelBase::GoToPage(PageType::Image, m_file);
    });
}


// Public member functions
void EditImageViewModel::CalculateRotationAngle(float delta)
{
    m_angle += (delta * 180 / M_PI);
    if (m_angle < 0)
    {
        m_angle += 360;
    }
    if (m_angle > 360)
    {
        m_angle -= 360;
    }
    OnPropertyChanged("Angle");
}

void EditImageViewModel::CalculateRotationAngle(float x, float y, double width, double height)
{
    CalculateRotationAngle(safe_cast<float>(atan2(y - height, x - width)));
}

void EditImageViewModel::GetCropStartCoordinates(Point onScreen, Point relative, int pageTitleRowHeight)
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

void EditImageViewModel::CalculateCropRectangleCoordinates(Point onScreen, Thickness imageMargin)
{
    // On-screen coordinates
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

void EditImageViewModel::CropImage(double actualWidth)
{
    m_isCropRectangleVisible = false;
    OnPropertyChanged("IsCropRectangleVisible");

    // TODO: Replace with BitmapBounds once in RC
    double scaleFactor = m_decoder->PixelWidth / actualWidth;

    int scaledWidth = safe_cast<int>(m_cropRectangleWidth * scaleFactor);
    int scaledHeight = safe_cast<int>(m_cropRectangleHeight * scaleFactor);
    int scaledXStart = safe_cast<int>(m_relativeCropAnchorPoint.X * scaleFactor);
    int scaledYStart = safe_cast<int>(m_relativeCropAnchorPoint.Y * scaleFactor);
    int j = scaledYStart;
    int i = scaledXStart;

    m_destinationPixels = ref new Array<unsigned char>((scaledWidth * scaledHeight) * 4);

    for (int y = 0; y < scaledHeight; y++)
    {
        for (int x = 0; x < scaledWidth; x++)
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

    InMemoryRandomAccessStream^ bitmapStream = ref new InMemoryRandomAccessStream();
    auto encodeTask = task<BitmapEncoder^>(BitmapEncoder::CreateAsync(BitmapEncoder::JpegEncoderId, bitmapStream));
    encodeTask.then([this, scaledWidth, scaledHeight, bitmapStream](BitmapEncoder^ encoder) {
        encoder->SetPixelData(BitmapPixelFormat::Rgba8, BitmapAlphaMode::Straight, m_newImageWidth, m_newImageHeight, m_decoder->DpiX, m_decoder->DpiY, m_destinationPixels);
        return encoder->FlushAsync();
    }).then([this, bitmapStream]() {
        m_image->SetSource(bitmapStream);
    }).then([this, bitmapStream]() {
        OnPropertyChanged("Photo");
        PopulatePixelArray(bitmapStream);
    });
}

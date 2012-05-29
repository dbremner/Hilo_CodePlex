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
#include "RotateImageViewModel.h"
#include "PhotoReader.h"
#include "TaskExceptionsExtensions.h"

#define EXIFOrientationPropertyName "System.Photo.Orientation"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Search;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;

RotateImageViewModel::RotateImageViewModel(IExceptionPolicy^ exceptionPolicy) : ImageBase(exceptionPolicy)
{
    m_rotateCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &RotateImageViewModel::Rotate90), nullptr);
    m_saveCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &RotateImageViewModel::SaveImage), nullptr);
    m_cancelCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &RotateImageViewModel::CancelRotate), nullptr);

    ViewModelBase::m_isAppBarSticky = true;
}

ICommand^ RotateImageViewModel::RotateCommand::get()
{
    return m_rotateCommand;
}

ICommand^ RotateImageViewModel::SaveCommand::get()
{
    return m_saveCommand;
}

ICommand^ RotateImageViewModel::CancelCommand::get()
{
    return m_cancelCommand;
}

Object^ RotateImageViewModel::ImageMargin::get()
{
    return m_imageMargin;
}

double RotateImageViewModel::RotationAngle::get()
{
    return m_rotationAngle;
}

void RotateImageViewModel::RotationAngle::set( double value )
{
    m_rotationAngle = value;

    // Derive margin so that rotated image is always fully shown on screen.
    Thickness margin(0.0);
    switch (safe_cast<unsigned int>(m_rotationAngle))
    {
    case 90:
    case 270:
        margin.Top = 110.0;
        margin.Bottom = 110.0;
        break;
    }
    m_imageMargin = margin;
    OnPropertyChanged("ImageMargin");
    OnPropertyChanged("RotationAngle");
}

ImageSource^ RotateImageViewModel::Photo::get()
{
    if (m_image == nullptr && m_file != nullptr)
    {
        auto fileOpenTask = task<ImageProperties^>(m_file->Properties->GetImagePropertiesAsync());
        fileOpenTask.then([this](ImageProperties^ properties)
        {
            auto requestedProperty = ref new Vector<String^>();
            requestedProperty->Append(EXIFOrientationPropertyName);
            return properties->RetrievePropertiesAsync(requestedProperty);
        }).then([this](IMap<String^, Object^>^ properties)
        {
            auto orientationProperty = dynamic_cast<IPropertyValue^>(properties->Lookup(EXIFOrientationPropertyName));
            if (orientationProperty != nullptr)
            {
                m_isExifOrientation = true;
                m_exifOrientation = orientationProperty->GetUInt16();
            }
            return m_file->OpenReadAsync();
        }).then([this](IRandomAccessStreamWithContentType^ imageData)
        {
            m_randomAccessStream = imageData;
            m_image = ref new BitmapImage();
            m_image->SetSource(m_randomAccessStream);
            OnPropertyChanged("Photo");
        }, task_continuation_context::use_current()).then(ObserveException<void>(m_exceptionPolicy));
    }
    return m_image;
}

void RotateImageViewModel::Rotate90(Object^ parameter)
{   
    RotationAngle += 90;
    EndRotation();
}

concurrency::task<void> RotateImageViewModel::DoRotate(double angle)
{
    assert(angle < 360);
    while (angle < 0)
    {
        angle += 360;
    }

    auto bitmapEncoder = std::make_shared<BitmapEncoder^>(nullptr);
    auto ras = ref new InMemoryRandomAccessStream();
    BitmapRotation rotation = static_cast<BitmapRotation>((int)floor(angle / 90));

    return task<BitmapDecoder^>(BitmapDecoder::CreateAsync(m_randomAccessStream))
        .then([ras](BitmapDecoder^ decoder)
    {
        return BitmapEncoder::CreateForTranscodingAsync(ras, decoder);
    }).then([this, rotation](BitmapEncoder^ encoder)
    {
        auto rotationTask = task<void>([]{});

        // If the file format supports EXIF orientation then update the orientation flag
        // to reflect any user-specified rotation. Otherwise, perform a hard rotate 
        // using the BitmapTransform class.
        if (m_isExifOrientation)
        {
            // Generate a new thumbnail to reflect any rotation operation
            encoder->IsThumbnailGenerated = true;

            auto currentExifOrientationDegrees = ConvertExifOrientationToDegreesRotation(m_exifOrientation);
            auto exifOrientationToApply = ConvertDegreesRotationToExifOrientation(safe_cast<unsigned int>(RotationAngle + currentExifOrientationDegrees));
            auto orientedTypedValue = ref new BitmapTypedValue(exifOrientationToApply, PropertyType::UInt16);
            auto properties = ref new Map<String^, BitmapTypedValue^>();
            properties->Insert(EXIFOrientationPropertyName, orientedTypedValue);

            return rotationTask.then([encoder, properties]()
            {
                return encoder->BitmapProperties->SetPropertiesAsync(properties);
            }).then([encoder]()
            {
                return encoder;
            });
        }
        return rotationTask.then([encoder, rotation]()
        {
            encoder->BitmapTransform->Rotation = rotation;
            return encoder;
        });
    }).then([bitmapEncoder](BitmapEncoder^ encoder)
    {        
        (*bitmapEncoder) = encoder;
        return encoder->FlushAsync();
    }).then([bitmapEncoder](task<void> encodeTask)
    {
        try
        {
            encodeTask.get();
        }
        catch(...)
        {
            // If the encoder does not support writing a thumbnail, then try again
            // but disable thumbnail generation.
            (*bitmapEncoder)->IsThumbnailGenerated = false;
            (*bitmapEncoder)->FlushAsync();
        }
    }).then([this, ras]()
    {
        m_randomAccessStream = ras;
        m_image->SetSource(m_randomAccessStream);
        RotationAngle = 0;
        OnPropertyChanged("Photo");
    });
}

void RotateImageViewModel::SaveImage(Object^ parameter)
{
    if (m_isSaving) return;

    m_isSaving = true;
    DoRotate(RotationAngle).then([this]()
    {
        return ImageBase::SaveImageAsync(m_file, m_randomAccessStream);
    }).then([this](task<void> savedTask)
    {
        try 
        {
            savedTask.get();
        }
        catch(...)
        {
            m_isSaving = false;
            throw;
        }
        m_isSaving = false;
    }).then(ObserveException<void>(m_exceptionPolicy));
}

void RotateImageViewModel::CancelRotate(Object^ parameter)
{
    ViewModelBase::GoBack();
}

void RotateImageViewModel::OnNavigatedTo(NavigationEventArgs^ e)
{
    Initialize(dynamic_cast<FileInformation^>(e->Parameter));
}

void RotateImageViewModel::Initialize(FileInformation^ image )
{
    m_file = image;
    assert(m_file!= nullptr);    
    m_image = nullptr;
}

void RotateImageViewModel::EndRotation()
{
    auto quarterTurns = (RotationAngle / 90);
    auto nearestQuarter = (int)floor(quarterTurns + 0.5) % 4;
    RotationAngle = (double)nearestQuarter * 90;
}

unsigned int RotateImageViewModel::ConvertExifOrientationToDegreesRotation(unsigned int exifOrientationFlag)
{
    switch (exifOrientationFlag)
    {
    case 1:
        return 0;
    case 6:
        return 90;
    case 3:
        return 180;
    case 8:
        return 270;
    default:
        // Ignore flip/mirroring values (2,4,5,7)
        return 0;
    }
}

unsigned int RotateImageViewModel::ConvertDegreesRotationToExifOrientation(unsigned int angle)
{
    switch (angle)
    {
    case 0:
        return 1;
    case 90:
        return 6;
    case 180:
        return 3;
    case 270:
        return 8;
    default:
        // Ignore flip/mirroring values (2,4,5,7)
        return 1;
    }
}
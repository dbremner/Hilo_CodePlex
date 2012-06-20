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
#include "IRepository.h"
#include "IPhoto.h"
#include "NullPhotoGroup.h"
#include "ImageNavigationData.h"
#include "SimpleQueryOperation.h"
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
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Search;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;

Platform::String^ const RotationStateKey = "rotation";
Platform::String^ const MarginLeftStateKey = "margin-left";
Platform::String^ const MarginTopStateKey = "margin-top";
Platform::String^ const MarginRightStateKey = "margin-right";
Platform::String^ const MarginBottomStateKey = "margin-bottom";

RotateImageViewModel::RotateImageViewModel(IRepository^ repository, IExceptionPolicy^ exceptionPolicy) : ImageBase(exceptionPolicy), m_repository(repository), m_imageMargin(Thickness(0.0))
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

Thickness RotateImageViewModel::ImageMargin::get()
{
    return m_imageMargin;
}

bool RotateImageViewModel::InProgress::get()
{
    return m_inProgress;
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

ImageSource^ RotateImageViewModel::Image::get()
{
    if (m_image == nullptr)
    {
        GetImagePhotoAsync().then([this] (IPhoto^ photo)
        {
            if (photo == nullptr)
            {
                cancel_current_task();
            }
            m_photo = photo;
            return m_photo->GetImagePropertiesAsync();
        }).then([this](ImageProperties^ properties)
        {
            auto requestedProperty = ref new Vector<String^>();
            requestedProperty->Append(EXIFOrientationPropertyName);
            return properties->RetrievePropertiesAsync(requestedProperty);
        }, task_continuation_context::use_arbitrary()).then([this](IMap<String^, Object^>^ properties)
        {
            auto orientationProperty = dynamic_cast<IPropertyValue^>(properties->Lookup(EXIFOrientationPropertyName));
            if (orientationProperty != nullptr)
            {
                m_isExifOrientation = true;
                m_exifOrientation = orientationProperty->GetUInt16();
            }
            return m_photo->OpenReadAsync();
        }).then([this](IRandomAccessStreamWithContentType^ imageData)
        {
            m_randomAccessStream = imageData;
            m_image = ref new BitmapImage();
            m_image->SetSource(m_randomAccessStream);
            OnPropertyChanged("Image");
        }).then(ObserveException<void>(m_exceptionPolicy));
    }
    return m_image;
}

void RotateImageViewModel::Rotate90(Object^ parameter)
{   
    RotationAngle += 90;
    EndRotation();
}

concurrency::task<void> RotateImageViewModel::RotateImageAsync(double angle)
{
    assert(angle < 360);
    while (angle < 0)
    {
        angle += 360;
    }

    auto continuationContext = task_continuation_context::use_arbitrary();
    auto bitmapEncoder = std::make_shared<BitmapEncoder^>(nullptr);
    auto ras = ref new InMemoryRandomAccessStream();
    BitmapRotation rotation = static_cast<BitmapRotation>((int)floor(angle / 90));

    return create_task(BitmapDecoder::CreateAsync(m_randomAccessStream))
        .then([ras](BitmapDecoder^ decoder)
    {
        return BitmapEncoder::CreateForTranscodingAsync(ras, decoder);
    }, continuationContext).then([this, rotation, continuationContext](BitmapEncoder^ encoder)
    {
        // If the file format supports EXIF orientation then update the orientation flag
        // to reflect any user-specified rotation. Otherwise, perform a hard rotate 
        // using the BitmapTransform class.
        if (m_isExifOrientation)
        {
            auto currentExifOrientationDegrees = ConvertExifOrientationToDegreesRotation(m_exifOrientation);
            auto exifOrientationToApply = ConvertDegreesRotationToExifOrientation(safe_cast<unsigned int>(RotationAngle + currentExifOrientationDegrees));
            auto orientedTypedValue = ref new BitmapTypedValue(exifOrientationToApply, PropertyType::UInt16);
            auto properties = ref new Map<String^, BitmapTypedValue^>();
            properties->Insert(EXIFOrientationPropertyName, orientedTypedValue);

            return create_task([]{}).then([encoder, properties]()
            {
                return encoder->BitmapProperties->SetPropertiesAsync(properties);
            }, continuationContext).then([encoder]()
            {
                return encoder;
            }, continuationContext);
        }
        return create_task([]{}).then([encoder, rotation]()
        {
            encoder->BitmapTransform->Rotation = rotation;
            return encoder;
        }, continuationContext);
    }, continuationContext).then([bitmapEncoder](BitmapEncoder^ encoder)
    {        
        // Force a new thumbnail to reflect any rotation operation
        encoder->IsThumbnailGenerated = true;
        (*bitmapEncoder) = encoder;
        return encoder->FlushAsync();
    }, continuationContext).then([bitmapEncoder](task<void> encodeTask)
    {
        try
        {
            encodeTask.get();
        }
        catch(Exception^ ex)
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
        m_randomAccessStream = ras;
    });
}

void RotateImageViewModel::ChangeInProgress(bool value)
{
    m_inProgress = value;
    OnPropertyChanged("InProgress");
}

void RotateImageViewModel::SaveImage(Object^ parameter)
{
    if (m_isSaving) return;

    m_isSaving = true;
    auto file = std::make_shared<StorageFile^>(nullptr);

    task<StorageFile^> saveTask = create_task(ImageBase::GetFileNameFromFileSavePickerAsync(m_photo->FileType));
    saveTask.then([this, file](StorageFile^ f)
    {
        ChangeInProgress(true);
        (*file) = f;
        return RotateImageAsync(RotationAngle);
    }).then([this, file]()
    {
        return ImageBase::SaveImageAsync((*file), m_randomAccessStream);
    }).then([this](task<void> priorTask)
    {
        m_isSaving = false;
        ChangeInProgress(false);
        try
        {
            priorTask.get();
        }
        catch(const concurrency::task_canceled&)
        {
        }
    }).then(ObserveException<void>(m_exceptionPolicy));
}

void RotateImageViewModel::CancelRotate(Object^ parameter)
{
    ViewModelBase::GoBack();
}

void RotateImageViewModel::LoadState(IMap<String^, Object^>^ stateMap) 
{
    if (stateMap != nullptr)
    {
        if (stateMap->HasKey(RotationStateKey))
            m_rotationAngle = static_cast<double>(stateMap->Lookup(RotationStateKey));

        Thickness margin(0.0);
        if (stateMap->HasKey(MarginRightStateKey))
            margin.Left = static_cast<double>(stateMap->Lookup(MarginRightStateKey));

        if (stateMap->HasKey(MarginTopStateKey))
            margin.Top = static_cast<double>(stateMap->Lookup(MarginTopStateKey));

        if (stateMap->HasKey(MarginRightStateKey))
            margin.Right = static_cast<double>(stateMap->Lookup(MarginRightStateKey));

        if (stateMap->HasKey(MarginBottomStateKey))
            margin.Bottom = static_cast<double>(stateMap->Lookup(MarginBottomStateKey));

        m_imageMargin = margin;
    }
}

void RotateImageViewModel::SaveState(IMap<String^, Object^>^ stateMap) 
{
    stateMap->Insert(RotationStateKey, m_rotationAngle);
    stateMap->Insert(MarginLeftStateKey, m_imageMargin.Left);
    stateMap->Insert(MarginTopStateKey, m_imageMargin.Top);
    stateMap->Insert(MarginRightStateKey, m_imageMargin.Right);
    stateMap->Insert(MarginBottomStateKey, m_imageMargin.Bottom);
}

void RotateImageViewModel::OnNavigatedTo(NavigationEventArgs^ e)
{
    String^ data = dynamic_cast<String^>(e->Parameter); 
    ImageNavigationData^ photoData = ref new ImageNavigationData(data);

    Initialize(photoData);
}


void RotateImageViewModel::Initialize(ImageNavigationData^ navigationData)
{
    assert(navigationData != nullptr);
    m_photo = nullptr;
    m_image = nullptr;
    m_navigationData = navigationData;
}

concurrency::task<IPhoto^> RotateImageViewModel::GetImagePhotoAsync()
{
    auto t = create_task(m_repository->GetPhotoForGroupWithQueryOperationAsync(ref new NullPhotoGroup(), ref new SimpleQueryOperation(m_navigationData->FilePath)));
    return t.then([this](IPhoto^ photo)
    {
        if (nullptr !=  photo)
        {
            return photo;
        }

        return static_cast<IPhoto^>(nullptr);
    });
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



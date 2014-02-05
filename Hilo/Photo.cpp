// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "Photo.h"
#include "PhotoImage.h"
#include "ExceptionPolicy.h"
#include "TaskExceptionsExtensions.h"
#include "CalendarExtensions.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace std;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::Storage;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Streams;
using namespace Windows::System::UserProfile;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Media::Imaging;

Photo::Photo(StorageFile^ fileInfo, IPhotoGroup^ photoGroup, shared_ptr<ExceptionPolicy> exceptionPolicy) : m_file(fileInfo), m_weakPhotoGroup(photoGroup), 
    m_exceptionPolicy(exceptionPolicy), 
    m_columnSpan(1), m_rowSpan(1), m_thumbnailStatus(UpdateStatus::Uninitialized)
{
}

Photo::~Photo()
{
}

void Photo::OnPropertyChanged(String^ propertyName)
{
    assert(IsMainThread());
    PropertyChanged(this, ref new PropertyChangedEventArgs(propertyName));
}

IPhotoGroup^ Photo::Group::get()
{
    return m_weakPhotoGroup.Resolve<IPhotoGroup>();
}

String^ Photo::Name::get()
{
    assert(IsMainThread());
    return m_file->Name;
}

String^ Photo::Path::get()
{
    return m_file->Path;
}

// Public wrapper (converts C++ types to Windows Runtime types)
IAsyncOperation<DateTime>^ Photo::GetDateTakenAsync()
{
    assert(IsMainThread());
    return create_async([this]() -> task<DateTime> { return GetDateTakenAsyncInternal(); });
}

// Calculates the "System.ItemDate" property. For images, this is the "DateTaken" image property,
// but if that property isn't available, the file's "DateModified" property is used instead. 
task<DateTime> Photo::GetDateTakenAsyncInternal()
{
    return create_task(m_file->Properties->GetImagePropertiesAsync()).then([this](task<ImageProperties^> imagePropertiesTask) -> task<DateTime>
    {
        bool sawException = false;
        DateTime dateTaken = {0ll};
        try
        {
            auto imageProperties = imagePropertiesTask.get();
            if (imageProperties != nullptr)
            {
                dateTaken = imageProperties->DateTaken;
            }
        }
        catch (Exception^)
        {
            sawException = true;
        }

        if (sawException || dateTaken.UniversalTime == 0) 
        {
            return create_task(m_file->GetBasicPropertiesAsync()).then([this](task<BasicProperties^> basicPropertiesTask) 
            { 
                BasicProperties^ basicProperties(nullptr);
                bool sawException = false;
                DateTime result = {0ll};
                try
                {
                    basicProperties = basicPropertiesTask.get();
                }
                catch (Exception^)
                {
                    sawException = true;
                }
                if (!sawException && basicProperties != nullptr)
                {
                    result = basicProperties->DateModified;
                }
                if (result.UniversalTime == 0ll)
                {
                    result = CalendarExtensions::GetCurrentDateTime();
                }
                return result;
            }, task_continuation_context::use_arbitrary());
        }
        else
        {
            return create_task_from_result<DateTime>(dateTaken);
        }
    }, task_continuation_context::use_arbitrary());
}

BitmapImage^ Photo::Thumbnail::get()
{
    assert(IsMainThread());
    if (m_thumbnailStatus ==  UpdateStatus::Uninitialized)
    {
        StartThumbnailGet();
    }
    return m_thumbnail;
}

bool Photo::IsInvalidThumbnail::get()
{
    return (m_thumbnailStatus == UpdateStatus::Error);
}

// Start getting the thumbnail for this image.
// Completion is signaled with a PropertyChanged event.
void Photo::StartThumbnailGet()
{
    assert(IsMainThread());
    assert(m_thumbnailStatus == UpdateStatus::Uninitialized);

    m_thumbnailStatus = UpdateStatus::Running;
    create_task(m_file->GetThumbnailAsync(ThumbnailMode::PicturesView, 190, ThumbnailOptions::UseCurrentScale)).then([this] (StorageItemThumbnail^ storageThumbnail) -> task<void>
    {
        assert(IsMainThread());
        IRandomAccessStream^ thumbnailStream = dynamic_cast<IRandomAccessStream^>(storageThumbnail);
        if (nullptr == thumbnailStream)
        {
            // Use default image if no thumbnail is available.
            UseDefaultThumbnail();
            return create_task([]{});
        }
        else
        {
            // Convert thumbnail file stream into a bitmap image that can be bound in XAML. 
            auto thumbnail = ref new BitmapImage(); 
            return create_task(thumbnail->SetSourceAsync(thumbnailStream)).then([this, thumbnail]()
            {
                assert(IsMainThread());
                m_thumbnail = thumbnail;
                m_thumbnailStatus = UpdateStatus::Ready;
                OnPropertyChanged("Thumbnail");
            }, task_continuation_context::use_current());
        }
    }, task_continuation_context::use_current()).then([this](task<void> priorTask)
    {
        try
        {
            priorTask.get();
        }
        // If GetThumbnailAsync or SetSourceAsync fails, just use the default thumbnail.
        catch(Exception^)
        {
            UseDefaultThumbnail();
            throw;
        }
    }, task_continuation_context::use_current()).then(ObserveException<void>(m_exceptionPolicy));
}

// When no thumbnail is available for this image file, use a default thumbnail.
void Photo::UseDefaultThumbnail()
{
    assert(IsMainThread());  
    m_thumbnail = ref new BitmapImage(ref new Uri("ms-appx:///Assets/HiloLogo.png"));
    m_thumbnailStatus = UpdateStatus::Error;
    OnPropertyChanged("Thumbnail");
    OnPropertyChanged("IsInvalidThumbnail");
}

int Photo::RowSpan::get()
{
    return m_rowSpan;
}

void Photo::RowSpan::set(int value)
{
    m_rowSpan = value;
}

int Photo::ColumnSpan::get()
{
    return m_columnSpan;
}

void Photo::ColumnSpan::set(int value)
{
    m_columnSpan = value;
}

IPhotoImage^ Photo::GetPhotoImage()
{
    return ref new PhotoImage(this, m_exceptionPolicy);
}

StorageFile^  Photo::File::get()
{
    return m_file;
}
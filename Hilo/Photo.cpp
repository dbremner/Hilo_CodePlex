//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "HiloCommonDefinitions.h"
#include "Photo.h"
#include "ExceptionPolicy.h"
#include "TaskExceptionsExtensions.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace std;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Streams;
using namespace Windows::System::UserProfile;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media::Imaging;

Photo::Photo(FileInformation^ fileInfo, IPhotoGroup^ photoGroup, shared_ptr<ExceptionPolicy> exceptionPolicy) : m_fileInfo(fileInfo), m_weakPhotoGroup(photoGroup), m_exceptionPolicy(exceptionPolicy), m_columnSpan(1), m_rowSpan(1), m_queryPhotoImageAsyncIsRunning(false)
{
    m_thumbnailUpdatedEventToken = m_fileInfo->ThumbnailUpdated += ref new TypedEventHandler<IStorageItemInformation^, Object^>(this, &Photo::OnThumbnailUpdated);
}

Photo::~Photo()
{
    if (nullptr != m_fileInfo)
    {
        m_fileInfo->ThumbnailUpdated -= m_thumbnailUpdatedEventToken;
    }
    ClearImageData();
}

void Photo::OnThumbnailUpdated(IStorageItemInformation^ sender, Object^ e)
{
    CoreWindow^ wnd = CoreApplication::MainView->CoreWindow;
    CoreDispatcher^ dispatcher = wnd->Dispatcher;
    dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this] () 
    {
        OnPropertyChanged("Thumbnail");
    }));
}

IPhotoGroup^ Photo::Group::get()
{
    return m_weakPhotoGroup.Resolve<IPhotoGroup>();
}

String^ Photo::Name::get()
{
    return m_fileInfo->Name;
}

String^ Photo::Path::get()
{
    return m_fileInfo->Path;
}

String^ Photo::FormattedPath::get()
{
    wstring pathAndFileName = m_fileInfo->Path->Data();
    basic_string<char>::size_type index;
    index = pathAndFileName.rfind('\\');
    wstring path = pathAndFileName.substr(0, index);
    return ref new String(path.c_str());
}

String^ Photo::FileType::get()
{
    return m_fileInfo->FileType;
}

DateTime Photo::DateTaken::get()
{
    auto dateTaken = m_fileInfo->ImageProperties->DateTaken;
    if (dateTaken.UniversalTime == 0)
    {
        dateTaken = m_fileInfo->BasicProperties->DateModified;
    }
    return dateTaken;
}

String^ Photo::FormattedDateTaken::get()
{
    DateTimeFormatter dtf("shortdate", GlobalizationPreferences::Languages);
    return dtf.Format(DateTaken);
}

String^ Photo::FormattedTimeTaken::get()
{
    DateTimeFormatter dtf("shorttime", GlobalizationPreferences::Languages);
    return dtf.Format(DateTaken);
}

String^ Photo::Resolution::get()
{
    return m_fileInfo->ImageProperties->Width + " x " + m_fileInfo->ImageProperties->Height;
}

unsigned long long Photo::FileSize::get()
{
    return m_fileInfo->BasicProperties->Size;
}

String^ Photo::DisplayType::get()
{
    return m_fileInfo->DisplayType;
}

BitmapImage^ Photo::Thumbnail::get()
{
    assert(IsMainThread());
    IRandomAccessStream^ thumbnailStream = dynamic_cast<IRandomAccessStream^>(m_fileInfo->Thumbnail);
    if (nullptr == thumbnailStream)
    {
        return ref new BitmapImage(ref new Uri("ms-appx:///Assets/HiloLogo.png"));
    }

    auto thumbnail = ref new BitmapImage();
    thumbnail->SetSource(thumbnailStream);
    return thumbnail;
}

BitmapImage^ Photo::Image::get()
{
    assert(IsMainThread());
    if (nullptr == m_image && !m_queryPhotoImageAsyncIsRunning)
    {
        m_queryPhotoImageAsyncIsRunning = true;
        QueryPhotoImageAsync().then([this](task<void> priorTask)
        {
            m_queryPhotoImageAsyncIsRunning = false;
            priorTask.get();
            OnPropertyChanged("Image");
        }).then(ObserveException<void>(m_exceptionPolicy));
    }
    return m_image;
}

task<void> Photo::QueryPhotoImageAsync()
{
    auto imageStreamTask = create_task(m_fileInfo->OpenReadAsync());
    return imageStreamTask.then([this](task<IRandomAccessStreamWithContentType^> antecedent)
    {
        assert(IsMainThread());
        try
        {
            IRandomAccessStreamWithContentType^ imageData = antecedent.get();
            m_image = ref new BitmapImage();
            m_imageFailedEventToken = m_image->ImageFailed += ref new ExceptionRoutedEventHandler(this, &Photo::OnImageFailedToOpen);
            m_image->SetSource(imageData);
        }
        catch (Exception^ e)
        {
            auto hr = e->HResult;
            if (hr == HILO_PHOTO_FILE_NOT_FOUND) 
            {
                OnImageFailedToOpen(nullptr, nullptr);
            }
            else
            {
                throw;
            }
        }
    });
}

void Photo::OnImageFailedToOpen(Object^ sender, ExceptionRoutedEventArgs^ e)
{
    assert(IsMainThread());
    m_image = ref new BitmapImage(ref new Uri("ms-appx:///Assets/HiloLogo.png"));
    OnPropertyChanged("Image");
}

void Photo::ClearImageData()
{
    if (nullptr != m_image && m_imageFailedEventToken.Value != 0)
    {
        m_image->ImageFailed -= m_imageFailedEventToken;
    }
    m_image = nullptr;
}

IAsyncOperation<ImageProperties^>^ Photo::GetImagePropertiesAsync()
{
    return m_fileInfo->Properties->GetImagePropertiesAsync();
}

IAsyncOperation<IRandomAccessStreamWithContentType^>^ Photo::OpenReadAsync()
{
    return m_fileInfo->OpenReadAsync();
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
//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "Photo.h"
#include "IPhotoGroup.h"
#include "IExceptionPolicy.h"
#include "TaskExceptionsExtensions.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Globalization;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Streams;
using namespace Windows::System::UserProfile;
using namespace Windows::UI::Xaml::Media::Imaging;

Photo::Photo(FileInformation^ fileInfo, IPhotoGroup^ photoGroup, IExceptionPolicy^ exceptionPolicy) : m_fileInfo(fileInfo), m_weakPhotoGroup(photoGroup), /*m_thumbnail(nullptr),*/ m_exceptionPolicy(exceptionPolicy), m_columnSpan(1), m_rowSpan(1)
{
    m_thumbnailUpdatedEventToken = m_fileInfo->ThumbnailUpdated += ref new TypedEventHandler<IStorageItemInformation^, Object^>(this, &Photo::OnThumbnailUpdated);
}

Photo::~Photo()
{
    if (nullptr != m_fileInfo)
    {
        m_fileInfo->ThumbnailUpdated -= m_thumbnailUpdatedEventToken;
    }
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
    std::wstring pathAndFileName = m_fileInfo->Path->Data();
    std::basic_string<char>::size_type index;
    index = pathAndFileName.rfind('\\');
    std::wstring path = pathAndFileName.substr(0, index);
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
    //return ref new BitmapImage();

    IRandomAccessStream^ thumbnailStream = dynamic_cast<IRandomAccessStream^>(m_fileInfo->Thumbnail);
    if (nullptr == thumbnailStream)
    {
        return nullptr;
    }

    auto thumbnail = ref new BitmapImage();
    thumbnail->SetSource(thumbnailStream);
    return thumbnail;

    /*if (nullptr == m_thumbnail)
    {
    auto createThumbnail = create_task(m_fileInfo->GetThumbnailAsync(ThumbnailMode::PicturesView));
    createThumbnail.then([this](StorageItemThumbnail^ thumbnail)
    {
    IRandomAccessStream^ thumbnailStream = static_cast<IRandomAccessStream^>(thumbnail);
    m_thumbnail = ref new BitmapImage();
    if (thumbnailStream != nullptr)
    {
    m_thumbnail->SetSource(thumbnailStream);
    OnPropertyChanged("Thumbnail");
    }
    }).then(ObserveException<void>(m_exceptionPolicy));
    }
    return m_thumbnail;*/
}

BitmapImage^ Photo::Image::get()
{
    if (nullptr == m_image)
    {
        auto createThumbnail = create_task(m_fileInfo->OpenReadAsync());
        createThumbnail.then([this](IRandomAccessStreamWithContentType^ imageData)
        {
            m_image = ref new BitmapImage();
            m_image->SetSource(imageData);
            OnPropertyChanged("Image");
        }).then(ObserveException<void>(m_exceptionPolicy));
    }
    return m_image;
}

void Photo::ClearImageData()
{
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

void Photo::OnThumbnailUpdated(Windows::Storage::BulkAccess::IStorageItemInformation^ sender, Platform::Object^ args)
{
    OnPropertyChanged("Thumbnail");
}
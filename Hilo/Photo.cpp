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
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Media::Imaging;

Photo::Photo(FileInformation^ fileInfo, IPhotoGroup^ photoGroup, IExceptionPolicy^ exceptionPolicy) : m_fileInfo(fileInfo), m_weakPhotoGroup(photoGroup), m_thumbnail(nullptr), m_exceptionPolicy(exceptionPolicy), m_columnSpan(1), m_rowSpan(1)
{
}

void Photo::OnThumbnailUpdated(IStorageItemInformation^ sender, Object^ e)
{
}

Photo::operator FileInformation^()
{
    return m_fileInfo;
}

Platform::String^ Photo::Name::get()
{
    return m_fileInfo->Name;
}

IPhotoGroup^ Photo::Group::get()
{
    return m_weakPhotoGroup.Resolve<IPhotoGroup>();
}

BitmapImage^ Photo::Thumbnail::get()
{
    if (nullptr == m_thumbnail)
    {
        task<StorageItemThumbnail^> createThumbnail(m_fileInfo->GetThumbnailAsync(ThumbnailMode::PicturesView));
        createThumbnail.then([this](StorageItemThumbnail^ thumbnail)
        {
            IRandomAccessStream^ thumbnailStream = static_cast<IRandomAccessStream^>(thumbnail);
            m_thumbnail = ref new BitmapImage();
            m_thumbnail->SetSource(thumbnailStream);
            OnPropertyChanged("Thumbnail");
        }, task_continuation_context::use_current()).then(ObserveException<void>(m_exceptionPolicy));
    }
    return m_thumbnail;
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

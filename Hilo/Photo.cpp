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
#include "PhotoGroup.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Media::Imaging;

Photo::Photo(FileInformation^ fileInfo, PhotoGroup^ photoGroup) : m_fileInfo(fileInfo), m_weakPhotoGroup(photoGroup), m_columnSpan(1), m_rowSpan(1), m_thumbnail(nullptr)
{
}

Platform::String^ Photo::Name::get()
{
    return m_fileInfo->Name;
}

Photo::operator IStorageFile^()
{
    return m_fileInfo;
}

PhotoGroup^ Photo::Group::get()
{
    return m_weakPhotoGroup.Resolve<PhotoGroup>();
}

BitmapImage^ Photo::Thumbnail::get()
{
    if (nullptr == m_thumbnail)
    {
        IRandomAccessStream^ thumbnailStream = dynamic_cast<IRandomAccessStream^>(m_fileInfo->Thumbnail);
        if (nullptr != thumbnailStream)
        {
            m_thumbnail = ref new BitmapImage();
            m_thumbnail->SetSource(thumbnailStream);
        }
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

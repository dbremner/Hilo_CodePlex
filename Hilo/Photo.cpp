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

using namespace Hilo;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Media::Imaging;

Photo::Photo(FileInformation^ fileInfo) : m_fileInfo(fileInfo), m_thumbnail(nullptr), m_columnSpan(1), m_rowSpan(1)
{
}

Platform::String^ Photo::Name::get()
{
    return m_fileInfo->Name;
}

BitmapImage^ Photo::Thumbnail::get()
{
    if (m_thumbnail == nullptr)
    {
        IRandomAccessStream^ thumbnailStream = dynamic_cast<IRandomAccessStream^>(m_fileInfo->Thumbnail);
        if (thumbnailStream != nullptr)
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

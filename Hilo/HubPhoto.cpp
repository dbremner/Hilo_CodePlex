//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "HubPhoto.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Media::Imaging;

HubPhoto::HubPhoto(FileInformation^ fileInfo) : m_fileInfo(fileInfo), m_thumbnail(nullptr), m_columnSpan(1), m_rowSpan(1)
{
}

Platform::String^ HubPhoto::Name::get()
{
    return m_fileInfo->Name;
}

HubPhoto::operator FileInformation^()
{
    return m_fileInfo;
}

BitmapImage^ HubPhoto::Thumbnail::get()
{
    if (m_thumbnail == nullptr)
    {
        try 
        {
            IRandomAccessStream^ thumbnailStream = dynamic_cast<IRandomAccessStream^>(m_fileInfo->Thumbnail);

            if (thumbnailStream != nullptr)
            {
                m_thumbnail = ref new BitmapImage();
                m_thumbnail->SetSource(thumbnailStream);
            }
        }
        catch(Exception^)
        {
            // If we have any problems reading the image, we just return the nullptr.
        }
    }
    return m_thumbnail;
}

int HubPhoto::RowSpan::get()
{
    return m_rowSpan;
}

void HubPhoto::RowSpan::set(int value)
{
    m_rowSpan = value;
}

int HubPhoto::ColumnSpan::get()
{
    return m_columnSpan;
}

void HubPhoto::ColumnSpan::set(int value)
{
    m_columnSpan = value;
}

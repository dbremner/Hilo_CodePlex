//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ImageViewData.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::Storage::BulkAccess;

ImageViewData::ImageViewData(FileInformation^ photo, Object^ photos) : m_photo(photo), m_photos(photos)
{
}

FileInformation^ ImageViewData::Photo::get()
{
    return m_photo;
}

Object^ ImageViewData::Photos::get()
{
    return m_photos;
}

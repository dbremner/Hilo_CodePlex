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
using namespace Windows::Storage;

ImageViewData::ImageViewData(IStorageFile^ photo, IStorageFolder^ folder) : m_photo(photo), m_folder(folder)
{
}

IStorageFile^ ImageViewData::Photo::get()
{
    return m_photo;
}

IStorageFolder^ ImageViewData::Folder::get()
{
    return m_folder;
}

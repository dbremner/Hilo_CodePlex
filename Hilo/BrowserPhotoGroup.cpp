//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"

#include "BrowserPhotoGroup.h"
#include "PhotoReader.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;

BrowserPhotoGroup::BrowserPhotoGroup(FolderInformation^ folderInformation) : m_folderInformation(folderInformation)
{
}

String^ BrowserPhotoGroup::Title::get()
{
    return m_folderInformation->Name;
}

Object^ BrowserPhotoGroup::Items::get()
{
    if (nullptr == m_photos)
    {
        PhotoReader reader;
        m_photos = reader.GetVirtualizedFiles(m_folderInformation, "");
    }
    return m_photos;
}
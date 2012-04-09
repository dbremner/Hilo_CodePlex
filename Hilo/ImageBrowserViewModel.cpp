//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ImageBrowserViewModel.h"
#include "PhotoReader.h"

using namespace Hilo;

using namespace Platform;

ImageBrowserViewModel::ImageBrowserViewModel()
{
}

// Properties
Object^ ImageBrowserViewModel::Photos::get()
{
    if (m_photos == nullptr)
    {
        PhotoReader reader;
        m_photos = reader.GetVirtualizedFiles("");
    }
    return m_photos;
}


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
#include "BrowserPhotoGroup.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::BulkAccess;

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

Object^ ImageBrowserViewModel::PhotoGroups::get()
{
    if (nullptr == m_photoGroups)
    {
        PhotoReader reader;
        auto photosTask = task<IVectorView<FolderInformation^>^>(reader.GetVirtualPhotoFoldersByMonth());
        photosTask.then([this](IVectorView<FolderInformation^>^ photos) 
        {
            auto tempPhotos = ref new Vector<Object^>();
            std::for_each(begin(photos), end(photos), [this, tempPhotos](FolderInformation^ folder) 
            {
                auto photoGroup = ref new BrowserPhotoGroup(folder);
                tempPhotos->Append(photoGroup);
            });
            m_photos = tempPhotos;
        }).then([this] 
        {
            OnPropertyChanged("PhotoGroups");
        },task_continuation_context::use_current());
    }
    return m_photos;
}

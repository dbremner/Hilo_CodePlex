//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "PhotoReader.h"
#include "ViewModelLocator.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform::Collections;
using namespace Windows::ApplicationModel::Resources;

MainHubViewModel^ ViewModelLocator::MainHubVM::get()
{
    if (nullptr == m_mainHubViewModel)
    {
        auto vector = ref new Vector<HubPhotoGroup^>();
        // Pictures Group
        auto picturesTask = create_async([]() 
        { 
            PhotoReader reader;
            return reader.GetPhotosAsync("", 6);
        });        
        auto picturesGroup = ref new HubPhotoGroup(picturesTask);
        auto loader = ref new ResourceLoader();
        auto title = loader->GetString("PicturesTitle");
        picturesGroup->Title = title;
        vector->Append(picturesGroup);
        m_mainHubViewModel = ref new MainHubViewModel(vector);
    }
    return m_mainHubViewModel;
}

ImageBrowserViewModel^ ViewModelLocator::ImageBrowserVM::get()
{
    if (nullptr == m_imageBrowswerViewModel)
    {
        m_imageBrowswerViewModel = ref new ImageBrowserViewModel();
    }
    return m_imageBrowswerViewModel;
}

ImageViewModel^ ViewModelLocator::ImageVM::get()
{
    if (nullptr == m_imageViewModel)
    {
        m_imageViewModel = ref new ImageViewModel();
    }
    return m_imageViewModel;
}

CropImageViewModel^ ViewModelLocator::CropImageVM::get()
{
    return ref new CropImageViewModel();
}

RotateImageViewModel^ ViewModelLocator::RotateImageVM::get()
{
    return ref new RotateImageViewModel();
}
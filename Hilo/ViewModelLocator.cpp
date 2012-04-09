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

ViewModelLocator::ViewModelLocator()
{
}

MainHubViewModel^ ViewModelLocator::MainHubVM::get()
{
    if (m_mainHubViewModel == nullptr)
    {
        auto vector = ref new Vector<PhotoGroup^>();
        // Pictures Group
        auto picturesTask = create_async([]() { 
            PhotoReader reader;
            return reader.GetPhotoGroupAsync("", 6);
        });        
        auto picturesGroup = ref new PhotoGroup(picturesTask);
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
    if (m_imageBrowswerViewModel == nullptr)
    {
        m_imageBrowswerViewModel = ref new ImageBrowserViewModel();
    }
    return m_imageBrowswerViewModel;
}

ImageViewModel^ ViewModelLocator::ImageVM::get()
{
    if (m_imageViewModel == nullptr)
    {
        m_imageViewModel = ref new ImageViewModel();
    }
    return m_imageViewModel;
}

EditImageViewModel^ ViewModelLocator::EditImageVM::get()
{
    return ref new EditImageViewModel();
}
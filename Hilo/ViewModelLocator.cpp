//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ViewModelLocator.h"
#include "DebugLoggingExceptionPolicy.h"
#include "FileRepository.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform::Collections;
using namespace Windows::ApplicationModel::Resources;

ViewModelLocator::ViewModelLocator()
{
    m_exceptionPolicy = ref new DebugLoggingExceptionPolicy();
}

MainHubViewModel^ ViewModelLocator::MainHubVM::get()
{
    if (nullptr == m_mainHubViewModel)
    {
        auto vector = ref new Vector<HubPhotoGroup^>();
        // Pictures Group
        auto loader = ref new ResourceLoader();
        auto title = loader->GetString("PicturesTitle");
        auto emptyTitle = loader->GetString("EmptyPicturesTitle");
        auto picturesGroup = ref new HubPhotoGroup(title, emptyTitle, ref new FileRepository(m_exceptionPolicy), m_exceptionPolicy);
        vector->Append(picturesGroup);
        m_mainHubViewModel = ref new MainHubViewModel(vector, m_exceptionPolicy);
    }
    return m_mainHubViewModel;
}

ImageBrowserViewModel^ ViewModelLocator::ImageBrowserVM::get()
{
    if (nullptr == m_imageBrowswerViewModel)
    {
        m_imageBrowswerViewModel = ref new ImageBrowserViewModel(ref new FileRepository(m_exceptionPolicy), m_exceptionPolicy);
    }
    return m_imageBrowswerViewModel;
}

ImageViewModel^ ViewModelLocator::ImageVM::get()
{
    if (nullptr == m_imageViewModel)
    {
        m_imageViewModel = ref new ImageViewModel(ref new FileRepository(m_exceptionPolicy), m_exceptionPolicy);
    }
    return m_imageViewModel;
}

CropImageViewModel^ ViewModelLocator::CropImageVM::get()
{
    return ref new CropImageViewModel(ref new FileRepository(m_exceptionPolicy), m_exceptionPolicy);
}

RotateImageViewModel^ ViewModelLocator::RotateImageVM::get()
{
    return ref new RotateImageViewModel(ref new FileRepository(m_exceptionPolicy), m_exceptionPolicy);
}
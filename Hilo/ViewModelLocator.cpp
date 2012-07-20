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
#include "FilePictureHubGroupQuery.h"
#include "FileSinglePhotoQuery.h"
#include "FileAllPhotosQuery.h"
#include "FileVirtualMonthFoldersQuery.h"
#include "FileVirtualYearFoldersQuery.h"

using namespace Hilo;
using namespace Platform::Collections;
using namespace std;
using namespace Windows::ApplicationModel::Resources;

ViewModelLocator::ViewModelLocator()
{
    m_exceptionPolicy = make_shared<DebugLoggingExceptionPolicy>();
}

MainHubViewModel^ ViewModelLocator::MainHubVM::get()
{
    auto vector = ref new Vector<HubPhotoGroup^>();
    // Pictures Group
    auto loader = ref new ResourceLoader();
    auto title = loader->GetString("PicturesTitle");
    auto emptyTitle = loader->GetString("EmptyPicturesTitle");
    auto picturesGroup = ref new HubPhotoGroup(title, emptyTitle, make_shared<FilePictureHubGroupQuery>(m_exceptionPolicy), m_exceptionPolicy);
    vector->Append(picturesGroup);
    return ref new MainHubViewModel(vector, m_exceptionPolicy);
}

ImageBrowserViewModel^ ViewModelLocator::ImageBrowserVM::get()
{
    if (nullptr == m_imageBrowswerViewModel)
    {
        m_imageBrowswerViewModel = ref new ImageBrowserViewModel(make_shared<FileVirtualMonthFoldersQuery>(m_exceptionPolicy), make_shared<FileVirtualYearFoldersQuery>(m_exceptionPolicy), m_exceptionPolicy);
    }
    return m_imageBrowswerViewModel;
}

ImageViewModel^ ViewModelLocator::ImageVM::get()
{
    return ref new ImageViewModel(make_shared<FileSinglePhotoQuery>(m_exceptionPolicy), make_shared<FileAllPhotosQuery>(m_exceptionPolicy), m_exceptionPolicy);
}

CropImageViewModel^ ViewModelLocator::CropImageVM::get()
{
    return ref new CropImageViewModel(make_shared<FileSinglePhotoQuery>(m_exceptionPolicy), m_exceptionPolicy);
}

RotateImageViewModel^ ViewModelLocator::RotateImageVM::get()
{
    return ref new RotateImageViewModel(make_shared<FileSinglePhotoQuery>(m_exceptionPolicy), m_exceptionPolicy);
}
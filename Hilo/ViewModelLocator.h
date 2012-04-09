//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "MainHubViewModel.h" // Required by generated header
#include "ImageBrowserViewModel.h" // Required by generated header
#include "ImageViewModel.h" // Required by generated header
#include "EditImageViewModel.h" // Required by generated header
#include "PhotoGroup.h" // Required by generated header

namespace Hilo
{
    interface class INavigationService;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class ViewModelLocator sealed
    {
    public:
        ViewModelLocator();

        property ImageBrowserViewModel^ ImageBrowserVM { ImageBrowserViewModel^ get(); }

        property MainHubViewModel^ MainHubVM { MainHubViewModel^ get(); }

        property ImageViewModel^ ImageVM { ImageViewModel^ get(); }

        property EditImageViewModel^ EditImageVM { EditImageViewModel^ get(); }

    private:
        ImageBrowserViewModel^ m_imageBrowswerViewModel;
        MainHubViewModel^ m_mainHubViewModel;
        ImageViewModel^ m_imageViewModel;
    };
}
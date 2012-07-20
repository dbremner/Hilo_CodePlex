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
#include "CropImageViewModel.h" // Required by generated header
#include "RotateImageViewModel.h" // Required by generated header
#include "HubPhotoGroup.h" // Required by generated header
#include "Photo.h" // Required by generated header
#include "MonthGroup.h" // Required by generated header
#include "YearGroup.h" // Required by generated header
#include "MonthBlock.h" // Required by generated header

namespace Hilo
{
    class ExceptionPolicy;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class ViewModelLocator sealed
    {
    public:
        ViewModelLocator();

        property ImageBrowserViewModel^ ImageBrowserVM { ImageBrowserViewModel^ get(); }
        property MainHubViewModel^ MainHubVM { MainHubViewModel^ get(); }
        property ImageViewModel^ ImageVM { ImageViewModel^ get(); }
        property CropImageViewModel^ CropImageVM { CropImageViewModel^ get(); }
        property RotateImageViewModel^ RotateImageVM { RotateImageViewModel^ get(); }

    private:
        ImageBrowserViewModel^ m_imageBrowswerViewModel;
        std::shared_ptr<ExceptionPolicy> m_exceptionPolicy;
    };
}
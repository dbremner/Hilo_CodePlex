//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "HiloPage.h"
#include "FileInformationToBitmapImageConverter.h" // Required by generated header
#include "FileInformationToToolTipConverter.h" // Required by generated header
#include "ImageView.g.h"

namespace Hilo
{
    public ref class ImageView sealed
    {
    public:
        ImageView();

    protected:
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
        virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

    private:
        void OnPhotosSelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
        void OnImageViewTopAppBarClosed(Platform::Object^ sender, Platform::Object^ e);
        void OnImageViewTopAppBarOpened(Platform::Object^ sender, Platform::Object^ e);

        Hilo::ImageViewModel^ m_viewModel;
    };
}

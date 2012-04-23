//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "HiloPage.h" // Required by generated header
#include "Common\BooleanToVisibilityConverter.h" // Required by generated header
#include "CropImageView.g.h"

namespace Hilo
{
    ref class CropImageViewModel;

	public ref class CropImageView sealed
	{
	public:
		CropImageView();

    protected:
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
        virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

    private:
        CropImageViewModel^ m_cropImageViewModel;
        bool m_pointerPressed;

        void OnImagePointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerEventArgs^ e);
        void OnImagePointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerEventArgs^ e);
        void OnImagePointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerEventArgs^ e);
        void OnCropRectangleTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
	};
}






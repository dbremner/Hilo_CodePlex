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
#include "EditImageView.g.h" 

namespace Hilo
{
    ref class EditImageViewModel;

    public ref class EditImageView sealed
    {
    public:
        EditImageView();

    protected:
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

    private:
        EditImageViewModel^ m_editImageViewModel;
        bool m_pointerPressed;

        // TODO: Replace event handlers with behaviors once supported in Blend.
        void OnImagePointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerEventArgs^ e);
        // TODO: Replace event handlers with behaviors once supported in Blend.
        void OnImagePointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerEventArgs^ e);
        // TODO: Replace event handlers with behaviors once supported in Blend.
        void OnImagePointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerEventArgs^ e);
        // TODO: Replace event handlers with behaviors once supported in Blend.
        void OnCropRectangleTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
    };
}

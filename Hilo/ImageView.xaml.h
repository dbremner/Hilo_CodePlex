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
#include "FileSizeConverter.h" // Required by generated header
#include "ImageView.g.h"

namespace Hilo
{
    public ref class ImageView sealed
    {
    public:
        ImageView();


    private:
        void OnFilmStripLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void OnImagePointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
        void OnImagePointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
        void OnImagePointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);

        Windows::Foundation::EventRegistrationToken m_filmStripLoadedToken;
        bool m_pointerPressed;
        Windows::Foundation::Point m_pointer;
    };
}

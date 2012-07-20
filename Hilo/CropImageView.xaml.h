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

    private:
        CropImageViewModel^ m_cropImageViewModel;
        bool m_sizeChangedAttached;

        void OnCropRectangleTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
        void OnThumbDragDelta(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::DragDeltaEventArgs^ e);
        void OnSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
    };
}






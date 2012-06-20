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
#include "RotateImageView.g.h"

namespace Hilo
{
    public ref class RotateImageView sealed
    {
    public:
        RotateImageView();

    protected:
        virtual void OnManipulationCompleted(Windows::UI::Xaml::Input::ManipulationCompletedRoutedEventArgs^ e) override;
        virtual void OnManipulationDelta(Windows::UI::Xaml::Input::ManipulationDeltaRoutedEventArgs^ e) override;


    private:
        RotateImageViewModel^ m_viewModel;
        void PhotoSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
    };
}

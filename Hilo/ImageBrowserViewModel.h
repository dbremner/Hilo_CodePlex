//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "ViewModelBase.h"

namespace Hilo
{
    [Windows::UI::Xaml::Data::Bindable]
    public ref class ImageBrowserViewModel sealed : public ViewModelBase
    {
    public:
        ImageBrowserViewModel();

        // Properties
        property Platform::Object^ Photos { Platform::Object^ get(); }

        property Platform::Object^ PhotoGroups
        {
            Platform::Object^ get();
        }

    private:
        Platform::Object^ m_photos;
        Windows::Foundation::Collections::IObservableVector<Platform::Object^>^ m_photoGroups;
    };
}

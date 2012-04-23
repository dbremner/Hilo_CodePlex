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
    ref class HubPhotoGroup;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class MainHubViewModel sealed : public ViewModelBase
    {
    public:
        MainHubViewModel(Windows::Foundation::Collections::IObservableVector<HubPhotoGroup^>^ photoGroups);

        property Platform::Object^ PhotoGroups
        {
            Object^ get();
        }

        property Windows::UI::Xaml::Input::ICommand^ NavigateToPicturesCommand
        {
            Windows::UI::Xaml::Input::ICommand^ get();
        }

    private:
        Windows::Foundation::Collections::IObservableVector<HubPhotoGroup^>^ m_photoGroups;
        Windows::UI::Xaml::Input::ICommand^ m_navigateToPicturesCommand;

        void NavigateToPictures(Platform::Object^ parameter);
    };
}

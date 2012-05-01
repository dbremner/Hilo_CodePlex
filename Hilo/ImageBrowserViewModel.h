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

        property Platform::Object^ PhotoGroups
        {
            Platform::Object^ get();
        }

        property bool InProgress
        {
            bool get();
        }

        property Windows::UI::Xaml::Input::ICommand^ GroupCommand
        {
            Windows::UI::Xaml::Input::ICommand^ get();
        }

    private:
        Platform::Collections::Vector<Platform::Object^>^ m_photoGroups;
        bool m_inProgress;
        Windows::UI::Xaml::Input::ICommand^ m_groupCommand;

        void NavigateToGroup(Platform::Object^ parameter);
    };
}

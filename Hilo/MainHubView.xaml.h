//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "pch.h"
#include "HiloPage.h" // Required by generated header
#include "VariableGridView.h" // Required by generated header
#include "VariableListView.h" // Required by generated header
#include "Common\BooleanToVisibilityConverter.h" // Required by generated header
#include "MainHubView.g.h"

namespace Hilo
{
    public ref class MainHubView sealed
    {
    public:
        MainHubView();
      
    protected:
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
        virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

    private:
        void OnPhotoItemClicked(Platform::Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
    };
}

//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ImageNavigationData.h"
#include "MainHubView.xaml.h"


using namespace Hilo;

using namespace Platform;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Navigation;

MainHubView::MainHubView()
{
    InitializeComponent();
}

void MainHubView::OnPhotoItemClicked(Object^ sender, ItemClickEventArgs^ e)
{
    auto photo = dynamic_cast<Photo^>(e->ClickedItem);
    if (nullptr !=  photo)
    {
        auto imageData = ref new ImageNavigationData(photo);
        HiloPage::NavigateToPage(PageType::Image, imageData->SerializeToString());
    }
}
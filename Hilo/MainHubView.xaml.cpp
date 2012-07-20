//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "MainHubView.xaml.h"
#include "ImageNavigationData.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::UI::Xaml::Controls;

MainHubView::MainHubView()
{
    InitializeComponent();
}

void MainHubView::OnPhotoItemClicked(Object^ sender, ItemClickEventArgs^ e)
{
    auto photo = dynamic_cast<Photo^>(e->ClickedItem);
    if (nullptr !=  photo)
    {
        ImageNavigationData imageData(photo);
        HiloPage::NavigateToPage(PageType::Image, imageData.SerializeToString());
    }
}
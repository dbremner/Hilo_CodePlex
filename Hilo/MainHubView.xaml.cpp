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

using namespace Hilo;

using namespace Platform;
using namespace Windows::UI::Xaml::Input;

MainHubView::MainHubView()
{
    InitializeComponent();
}


void MainHubView::OnItemGridViewTapped(Object^ sender, TappedRoutedEventArgs^ e)
{
    HiloPage::NavigateToPage(PageType::Browse, nullptr);
}

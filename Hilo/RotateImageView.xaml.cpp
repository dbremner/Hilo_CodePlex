//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "RotateImageView.xaml.h"

using namespace Hilo;

using namespace Windows::UI::Xaml::Navigation;

RotateImageView::RotateImageView()
{
    InitializeComponent();
}

void RotateImageView::OnNavigatedTo(NavigationEventArgs^ e)
{
    HiloPage::OnNavigatedTo(e);
}

void RotateImageView::OnNavigatedFrom(NavigationEventArgs^ e)
{
    HiloPage::OnNavigatedFrom(e);
}


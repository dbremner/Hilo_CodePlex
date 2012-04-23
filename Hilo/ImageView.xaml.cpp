//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ImageViewModel.h"
#include "ImageView.xaml.h"

using namespace Hilo;

using namespace Platform;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Navigation;

ImageView::ImageView()
{
    InitializeComponent();
}

void ImageView::OnNavigatedTo(NavigationEventArgs^ e)
{
    HiloPage::OnNavigatedTo(e);
}

void ImageView::OnNavigatedFrom(NavigationEventArgs^ e)
{
    HiloPage::OnNavigatedFrom(e);
}
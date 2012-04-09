//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ImageBrowserViewModel.h"
#include "ImageViewData.h"
#include "ImageBrowserView.xaml.h"

using namespace Hilo;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::ViewManagement;
using namespace Windows::Graphics::Display;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;

ImageBrowserView::ImageBrowserView()
{
    InitializeComponent();
}

void Hilo::ImageBrowserView::OnPhotoItemClicked(Platform::Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e)
{
    auto file = dynamic_cast<FileInformation^>(e->ClickedItem);
    auto viewModel = static_cast<ImageBrowserViewModel^>(DataContext);
    auto photos = viewModel->Photos;
    auto imageData = ref new ImageViewData(file, photos);
    if (file != nullptr)
    {
        HiloPage::NavigateToPage(PageType::Image, imageData);
    }
}
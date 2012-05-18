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
using namespace Windows::Graphics::Display;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::ViewManagement;

ImageBrowserView::ImageBrowserView()
{
    InitializeComponent();
}

void ImageBrowserView::OnNavigatedTo(NavigationEventArgs^ e)
{
    HiloPage::OnNavigatedTo(e);
}

void ImageBrowserView::OnNavigatedFrom(NavigationEventArgs^ e)
{
    HiloPage::OnNavigatedFrom(e);
}

void ImageBrowserView::OnPhotoItemClicked(Object^ sender, ItemClickEventArgs^ e)
{
    auto photo = dynamic_cast<Photo^>(e->ClickedItem);
    if (nullptr !=  photo)
    {
        auto imageData = ref new ImageViewData(photo);
        HiloPage::NavigateToPage(PageType::Image, imageData);
    }
}

void ImageBrowserView::OnViewChangeCompleted(Object^ sender, SemanticZoomViewChangedEventArgs^ e)
{
    if (!e->IsSourceZoomedInView)
    {
        auto viewModel = dynamic_cast<ImageBrowserViewModel^>(DataContext);
        auto monthBlock = dynamic_cast<MonthBlock^>(e->SourceItem->Item);
        if (nullptr != monthBlock)
        {
            auto photo = viewModel->GetPhotoForYearAndMonth(monthBlock->Group->Year, monthBlock->Month);
            if (nullptr != photo)
            {
                MonthPhotosGridView->ScrollIntoView(photo);
            }
        }
    }
}

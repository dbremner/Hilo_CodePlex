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
#include "ImageNavigationData.h"
#include "ImageBrowserView.xaml.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::UI::Xaml::Controls;

ImageBrowserView::ImageBrowserView()
{
    InitializeComponent();
}

void ImageBrowserView::OnPhotoItemClicked(Object^ sender, ItemClickEventArgs^ e)
{
    auto photo = dynamic_cast<Photo^>(e->ClickedItem);
    if (nullptr !=  photo)
    {
        ImageNavigationData imageData(photo);
        HiloPage::NavigateToPage(PageType::Image, imageData.SerializeToString());
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

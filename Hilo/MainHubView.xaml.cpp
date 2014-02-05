// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "ExceptionPolicyFactory.h"
#include "MainHubView.xaml.h"
#include "ImageNavigationData.h"


using namespace Hilo;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Controls;
using namespace concurrency;

// See http://go.microsoft.com/fwlink/?LinkId=267278 for info on how Hilo creates pages and navigates to pages.

MainHubView::MainHubView()
{
    InitializeComponent();
}

void MainHubView::OnPhotoItemClicked(Object^ sender, ItemClickEventArgs^ e)
{
    auto photo = dynamic_cast<IPhoto^>(e->ClickedItem);
    if (nullptr !=  photo)
    {
        create_task(photo->GetDateTakenAsync()).then([photo, this](DateTime dateTaken) 
        {
            assert(IsMainThread());
            ImageNavigationData imageData(photo->Path, dateTaken);
            HiloPage::NavigateToPage(PageType::Image, imageData.SerializeToString());
        }, task_continuation_context::use_current()).then(ObserveException<void>(ExceptionPolicyFactory::GetCurrentPolicy())); 
    }
}
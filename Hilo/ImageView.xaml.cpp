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
using namespace Windows::Storage::BulkAccess;
using namespace Windows::UI::Xaml::Navigation;

ImageView::ImageView()
{
    InitializeComponent();
    m_viewModel = dynamic_cast<ImageViewModel^>(DataContext);
}

void ImageView::OnNavigatedTo(NavigationEventArgs^ e)
{
    HiloPage::OnNavigatedTo(e);
}

void ImageView::OnNavigatedFrom(NavigationEventArgs^ e)
{
    HiloPage::OnNavigatedFrom(e);
}

void ImageView::OnPhotosSelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
    auto senderString = sender->ToString();
    if (senderString->Equals("Windows.UI.Xaml.Controls.FlipView"))
    {
        m_viewModel->SelectedItem = dynamic_cast<FileInformation^>(PhotosFlipView->SelectedItem);
    }
    else if (senderString->Equals("Windows.UI.Xaml.Controls.GridView"))
    {
        PhotosFlipView->SelectedIndex = PhotosFilmStripGridView->SelectedIndex;
    }
}

void ImageView::OnImageViewTopAppBarOpened(Platform::Object^ sender, Platform::Object^ e)
{
    ImageViewFileInformationPopup->IsOpen = true;
}

void ImageView::OnImageViewTopAppBarClosed(Platform::Object^ sender, Platform::Object^ e)
{
    ImageViewFileInformationPopup->IsOpen = false;
}

//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ImageView.xaml.h"
#include "ImageViewModel.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::UI::Input;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Input;

ImageView::ImageView()
{
    InitializeComponent();

    m_filmStripLoadedToken = PhotosFilmStripGridView->Loaded += ref new RoutedEventHandler(this, &ImageView::OnFilmStripLoaded);
}

ImageView::~ImageView()
{
    if (nullptr != PhotosFilmStripGridView)
    {
        PhotosFilmStripGridView->Loaded -= m_filmStripLoadedToken;
    }
}

// Scrolls the selected item into view after the collection is likely to have loaded.
void ImageView::OnFilmStripLoaded(Object^ sender, RoutedEventArgs^ e)
{
    auto vm = dynamic_cast<ImageViewModel^>(DataContext);
    if (vm != nullptr)
    {
        PhotosFilmStripGridView->ScrollIntoView(vm->SelectedItem);
    }

    PhotosFilmStripGridView->Loaded -= m_filmStripLoadedToken;
}

void Hilo::ImageView::OnImagePointerPressed(Object^ sender, PointerRoutedEventArgs^ e)
{
    m_pointerPressed = true;
    PointerPoint^ point = e->GetCurrentPoint(PhotoGrid);
    m_pointer = point->Position;
    if (point->Properties->IsLeftButtonPressed)
    {
        ImageViewFileInformationPopup->HorizontalOffset = point->Position.X - 200;
        ImageViewFileInformationPopup->VerticalOffset = point->Position.Y - 200;
        ImageViewFileInformationPopup->IsOpen = true;
    }
}

void Hilo::ImageView::OnImagePointerReleased(Object^ sender, PointerRoutedEventArgs^ e)
{
    if (m_pointerPressed)
    {
        ImageViewFileInformationPopup->IsOpen = false;
        m_pointerPressed = false;
    }
}

void Hilo::ImageView::OnImagePointerMoved(Object^ sender, PointerRoutedEventArgs^ e)
{
    if (m_pointerPressed)
    {
        PointerPoint^ point = e->GetCurrentPoint(PhotoGrid);
        if (point->Position != m_pointer)
        {
            OnImagePointerReleased(sender, e);
        }
    }
}

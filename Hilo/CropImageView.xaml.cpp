//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "CropImageViewModel.h"
#include "CropImageView.xaml.h"

using namespace Hilo;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Navigation;

CropImageView::CropImageView()
{
	InitializeComponent();
    m_cropImageViewModel = static_cast<CropImageViewModel^>(DataContext);
}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.  The Parameter
/// property is typically used to configure the page.</param>
void CropImageView::OnNavigatedTo(NavigationEventArgs^ e)
{
    HiloPage::OnNavigatedTo(e);
}

void CropImageView::OnNavigatedFrom(NavigationEventArgs^ e)
{
    HiloPage::OnNavigatedFrom(e);
}

void CropImageView::OnPhotoSizeChanged(Platform::Object^ sender, SizeChangedEventArgs^ e)
{
    m_cropImageViewModel->CalculateInitialCropOverlayPosition(
        Photo->TransformToVisual(CropImageGrid), 
        Photo->RenderSize.Width, Photo->RenderSize.Height);
}

void CropImageView::OnThumbDragDelta(Platform::Object^ sender, DragDeltaEventArgs^ e)
{
    m_cropImageViewModel->UpdateCropOverlayPostion(safe_cast<Thumb^>(sender), 
        e->VerticalChange, 
        e->HorizontalChange, 
        CropOverlay->MinWidth, CropOverlay->MinHeight);
}

void CropImageView::OnCropRectangleTapped(Object^ sender, TappedRoutedEventArgs^ e)
{
    if (!m_cropImageViewModel->InProgress)
    {
        m_cropImageViewModel->CropImage(Photo->ActualWidth);
    }
}
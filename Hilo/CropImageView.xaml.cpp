//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "CropImageView.xaml.h"
#include "CropImageViewModel.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Input;

CropImageView::CropImageView()
{
    InitializeComponent();
    m_cropImageViewModel = static_cast<CropImageViewModel^>(DataContext);
    m_sizeChangedAttached = false;
}

void CropImageView::OnSizeChanged(Object^ sender, SizeChangedEventArgs^ e)
{
    m_cropImageViewModel->CalculateInitialCropOverlayPosition(
        Photo->TransformToVisual(CropImageGrid), 
        Photo->RenderSize.Width, Photo->RenderSize.Height);

    if (!m_sizeChangedAttached)
    {
        SizeChanged += ref new SizeChangedEventHandler(this, &CropImageView::OnSizeChanged);
        m_sizeChangedAttached = true;
    }
}

void CropImageView::OnThumbDragDelta(Object^ sender, DragDeltaEventArgs^ e)
{
    if (!m_cropImageViewModel->InProgress)
    {
        m_cropImageViewModel->UpdateCropOverlayPostion(safe_cast<Thumb^>(sender), 
            e->VerticalChange, 
            e->HorizontalChange, 
            CropOverlay->MinWidth, CropOverlay->MinHeight);
    }
}

void CropImageView::OnCropRectangleTapped(Object^ sender, TappedRoutedEventArgs^ e)
{
    if (!m_cropImageViewModel->InProgress)
    {
        m_cropImageViewModel->CropImageAsync(Photo->ActualWidth);
    }
}

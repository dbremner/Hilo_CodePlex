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

void CropImageView::OnImagePointerPressed(Object^ sender, PointerEventArgs^ e)
{
    m_pointerPressed = true;
    m_cropImageViewModel->GetCropStartCoordinates(e->GetCurrentPoint(nullptr)->Position, e->GetCurrentPoint(Photo)->Position, safe_cast<int>(PageTitleRow->ActualHeight));    
    CropBorderAnimation->Stop();
}

void CropImageView::OnImagePointerReleased(Object^ sender, PointerEventArgs^ e)
{
    m_pointerPressed = false;
    CropBorderAnimation->Begin();
}

void CropImageView::OnImagePointerMoved(Object^ sender, PointerEventArgs^ e)
{
    if (m_pointerPressed)
    {
        m_cropImageViewModel->CalculateCropRectangleCoordinates(e->GetCurrentPoint(nullptr)->Position, e->GetCurrentPoint(Photo)->Position, Photo->Margin);
    }
}

void CropImageView::OnCropRectangleTapped(Object^ sender, TappedRoutedEventArgs^ e)
{
    m_cropImageViewModel->CropImage(Photo->ActualWidth);
}

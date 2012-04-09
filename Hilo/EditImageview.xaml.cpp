//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "EditImageViewModel.h" 
#include "EditImageView.xaml.h"

using namespace Hilo;

using namespace Platform;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Navigation;

EditImageView::EditImageView()
{
	InitializeComponent();
    m_editImageViewModel = static_cast<EditImageViewModel^>(DataContext);
}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.  The Parameter
/// property is typically used to configure the page.</param>
void EditImageView::OnNavigatedTo(NavigationEventArgs^ e)
{
    HiloPage::OnNavigatedTo(e);
}

void EditImageView::OnImagePointerPressed(Object^ sender, PointerEventArgs^ e)
{
    m_pointerPressed = true;
    m_editImageViewModel->GetCropStartCoordinates(e->GetCurrentPoint(nullptr)->Position, e->GetCurrentPoint(Photo)->Position, safe_cast<int>(PageTitleRow->ActualHeight));    
    CropBorderAnimation->Stop();
}

void EditImageView::OnImagePointerReleased(Object^ sender, PointerEventArgs^ e)
{
    m_pointerPressed = false;
    CropBorderAnimation->Begin();
}

void EditImageView::OnImagePointerMoved(Object^ sender, PointerEventArgs^ e)
{
    if (m_pointerPressed)
    {
        m_editImageViewModel->CalculateCropRectangleCoordinates(e->GetCurrentPoint(nullptr)->Position, Photo->Margin);
    }
}

void EditImageView::OnCropRectangleTapped(Object^ sender, TappedRoutedEventArgs^ e)
{
    m_editImageViewModel->CropImage(Photo->ActualWidth);
}

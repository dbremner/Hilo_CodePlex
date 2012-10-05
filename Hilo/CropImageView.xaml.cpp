#include "pch.h"
#include "CropImageView.xaml.h"
#include "CropImageViewModel.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Input;

// See http://go.microsoft.com/fwlink/?LinkId=267278 for info on how Hilo creates pages and navigates to pages.

CropImageView::CropImageView()
{
    InitializeComponent();
    m_cropImageViewModel = static_cast<CropImageViewModel^>(DataContext);
    m_sizeChangedAttached = false;
}

// <snippet708>
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
// </snippet708>

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

// <snippet1206>
void CropImageView::OnCropRectangleTapped(Object^ sender, TappedRoutedEventArgs^ e)
{
    if (!m_cropImageViewModel->InProgress)
    {
        m_cropImageViewModel->CropImageAsync(Photo->ActualWidth);
    }
}
// </snippet1206>

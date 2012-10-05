#include "pch.h"
#include "RotateImageView.xaml.h"
#include "ImageBrowserViewModel.h"

using namespace Hilo;
using namespace Windows::Foundation;
using namespace Windows::UI::Input;
using namespace Windows::UI::Xaml::Input;

// See http://go.microsoft.com/fwlink/?LinkId=267278 for info on how Hilo creates pages and navigates to pages.

RotateImageView::RotateImageView() 
{
    InitializeComponent();
    m_viewModel = dynamic_cast<Hilo::RotateImageViewModel^>(DataContext);
}

// <snippet1213>
void RotateImageView::OnManipulationDelta(ManipulationDeltaRoutedEventArgs^ e)
{
    m_viewModel->RotationAngle += e->Delta.Rotation;
}

void RotateImageView::OnManipulationCompleted(ManipulationCompletedRoutedEventArgs^ e)
{
    m_viewModel->EndRotation();
}
// </snippet1213>
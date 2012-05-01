//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "RotateImageView.xaml.h"
#include "ImageBrowserViewModel.h"

#define _USE_MATH_DEFINES
#include <math.h>


using namespace Hilo;

using namespace Windows::UI::Xaml::Navigation;

// This works around an issue with compiling for ARM.
// This function does not need to be called but needs to be
// seen by the compiler beforet the gesture manipulation events.
void gestureRecognizerWorkaroundForARM()
{
    Windows::Foundation::Point pt;
    Windows::UI::Input::ManipulationVelocities vel;
    Windows::UI::Input::CrossSlideThresholds thres;
    (void)pt;
    (void)vel;
    (void)thres;
}

RotateImageView::RotateImageView() 
{
    InitializeComponent();
    m_viewModel = dynamic_cast<Hilo::RotateImageViewModel^>(DataContext);
    Photo->SizeChanged += ref new Windows::UI::Xaml::SizeChangedEventHandler(this, &RotateImageView::PhotoSizeChanged);
}

void RotateImageView::OnNavigatedTo(NavigationEventArgs^ e)
{
    HiloPage::OnNavigatedTo(e);
}

void RotateImageView::OnNavigatedFrom(NavigationEventArgs^ e)
{
    HiloPage::OnNavigatedFrom(e);
}

void RotateImageView::PhotoSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
    // Normally, we would try to do this in XAML binding, but the ActualWidth and ActualHeight don''
    // appear to bind correctly to the transform with {Binding ElementName=Photo, Path=ActualX,...}
    auto width = Photo->ActualWidth;
    auto height = Photo->ActualHeight;

    ImageRotateTransform->CenterX = width / 2;
    ImageRotateTransform->CenterY = height / 2;
}

void RotateImageView::OnManipulationDelta( Windows::UI::Xaml::Input::ManipulationDeltaRoutedEventArgs^ e )
{
    // Set rotation angle (convert to degrees from radians)
    // Note: This will change for CP which will then report degrees instead of radians
    m_viewModel->RotationAngle += (e->Delta.Rotation * 180) / (float)M_PI;
}

void RotateImageView::OnManipulationCompleted( Windows::UI::Xaml::Input::ManipulationCompletedRoutedEventArgs^ e )
{
    m_viewModel->EndRotation();
}

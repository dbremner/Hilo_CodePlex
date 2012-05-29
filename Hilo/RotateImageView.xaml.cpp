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

using namespace Windows::Foundation;
using namespace Windows::UI::Input;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Navigation;
using namespace Platform;

// This works around an issue with compiling for ARM.
// This function does not need to be called but needs to be
// seen by the compiler beforet the gesture manipulation events.
void gestureRecognizerWorkaroundForARM()
{
    Point pt;
    ManipulationVelocities vel;
    CrossSlideThresholds thres;
    (void)pt;
    (void)vel;
    (void)thres;
}

RotateImageView::RotateImageView() 
{
    InitializeComponent();
    m_viewModel = dynamic_cast<Hilo::RotateImageViewModel^>(DataContext);
}

void RotateImageView::OnNavigatedTo(NavigationEventArgs^ e)
{
    HiloPage::OnNavigatedTo(e);
}

void RotateImageView::OnNavigatedFrom(NavigationEventArgs^ e)
{
    HiloPage::OnNavigatedFrom(e);
}

void RotateImageView::OnManipulationDelta(ManipulationDeltaRoutedEventArgs^ e)
{
    // Set rotation angle (convert to degrees from radians)
    // Note: This will change for CP which will then report degrees instead of radians
    m_viewModel->RotationAngle += (e->Delta.Rotation * 180) / (float)M_PI;
}

void RotateImageView::OnManipulationCompleted(ManipulationCompletedRoutedEventArgs^ e)
{
    m_viewModel->EndRotation();
}

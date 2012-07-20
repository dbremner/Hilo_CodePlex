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

using namespace Hilo;
using namespace Windows::Foundation;
using namespace Windows::UI::Input;
using namespace Windows::UI::Xaml::Input;

// This works around an issue with compiling for ARM.
// This function does not need to be called but needs to be
// seen by the compiler before the gesture manipulation events.
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

void RotateImageView::OnManipulationDelta(ManipulationDeltaRoutedEventArgs^ e)
{
    m_viewModel->RotationAngle += e->Delta.Rotation ;
}

void RotateImageView::OnManipulationCompleted(ManipulationCompletedRoutedEventArgs^ e)
{
    m_viewModel->EndRotation();
}

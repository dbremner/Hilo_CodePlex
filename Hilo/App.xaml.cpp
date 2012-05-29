//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "MainHubView.xaml.h"
#include "TileUpdateScheduler.h"
#include "ExceptionPolicyFactory.h"

using namespace Hilo;
using namespace Platform;
using namespace Platform::Details;
using namespace Platform::Collections;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App()
{
    InitializeComponent();
    Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);
    UnhandledException += ref new UnhandledExceptionEventHandler(this, &App::OnUnhandledException);
}

/// <summary>
/// Invoked when the application is launched normally by the end user.  Other entry points
/// will be used when the application is launched to open a specific file, to display
/// search results, and so forth.
/// </summary>
/// <param name="args">Details about the launch request and process.</param>
void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ pArgs)
{
    m_exceptionPolicy = ExceptionPolicyFactory::GetCurrentPolicy();

    if (pArgs->PreviousExecutionState == ApplicationExecutionState::Terminated)
    {
    }

    // Schedule tile updates.
    TileUpdateScheduler scheduler(m_exceptionPolicy);
    scheduler.ScheduleUpdateAsync();

    // Create a Frame to act navigation context and navigate to the first page
    auto rootFrame = ref new Frame();
    TypeName mainHubType = { MainHubView::typeid->FullName, TypeKind::Metadata };
    rootFrame->Navigate(mainHubType);

    // Place the frame in the current Window and ensure that it is active
    Window::Current->Content = rootFrame;
    Window::Current->Activate();
}

/// <summary>
/// Invoked when application execution is being suspended.  Application state is saved
/// without knowing whether the application will be terminated or resumed with the contents
/// of memory still intact.
/// </summary>
/// <param name="sender">The source of the suspend request.</param>
/// <param name="e">Details about the suspend request.</param>
void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{
}

void App::OnUnhandledException(Object^ sender, UnhandledExceptionEventArgs^ args)
{
    auto exception = ref new FailureException();
    m_exceptionPolicy->HandleException(exception);
    
    auto e = args->Exception;
    auto m = args->Message;

    throw exception;
}
//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "DispatchedEvent.h"

using namespace HiloTests;
using namespace concurrency;

DispatchedEvent::DispatchedEvent(void)
{
}

void DispatchedEvent::set()
{
    m_event.set();
}

void DispatchedEvent::wait()
{
    // Spin wait and exercise message pump
    auto waitResult = COOPERATIVE_WAIT_TIMEOUT;
    Windows::UI::Core::CoreWindow^ wnd = Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow;
    Windows::UI::Core::CoreDispatcher^ dispatcher = wnd->Dispatcher;

    while(waitResult != 0)
    {
        dispatcher->ProcessEvents(Windows::UI::Core::CoreProcessEventsOption::ProcessAllIfPresent);
        waitResult = m_event.wait(0);
    }
}
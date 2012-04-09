//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "TestHelper.h"

using namespace HiloTests;

using namespace concurrency;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Windows::Foundation;

void TestHelper::RunUIAsync(std::function<void ()>&& action)
{
    Windows::UI::Core::CoreWindow^ wnd = Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow;

    if (wnd != nullptr)
    {
        Platform::String^ errorMessage = nullptr;

        auto callback = ref new Windows::UI::Core::DispatchedHandler(
            [&errorMessage, action]()
        {
            try 
            {
                action();
            } 
            catch (Platform::Exception^ ex) 
            {
                errorMessage = ex->Message;
            } 
            catch (...) 
            {
                errorMessage = ref new Platform::String(L"Exception occured in code");
            }

        });

        task<void> uiTask(wnd->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
            callback));
        task_status status = TaskWait<void>(uiTask);
        
        if (status == not_complete)
        {
            Assert::Fail(L"Task did not complete");
        }

        if (errorMessage != nullptr) 
        {
            Assert::Fail(errorMessage->Data());
        }
    }
}

template<typename T>
task_status TestHelper::TaskWait(task<T>& t, bool pump)
{
    HANDLE hEvent = CreateEventEx(NULL, NULL, CREATE_EVENT_MANUAL_RESET, WRITE_OWNER | EVENT_ALL_ACCESS);
    if (hEvent == NULL)
    {
        throw std::bad_alloc();
    }

    task_status status = not_complete;
    t.then([&hEvent, &status](task<T> result){
        try
        {
            result.get();
            status = completed;
        }
        catch(...)
        {
            status = canceled;
        }
        SetEvent(hEvent);
    });

    if (pump)
    {
        //WaitForMultipleObjectsEx(1, &hEvent, true, INFINITE, true);
        DWORD dummy;
        CoWaitForMultipleHandles(0x8, INFINITE, 1, &hEvent, &dummy);
    }
    else
    {
        WaitForSingleObjectEx(hEvent, INFINITE, true);
    }
    CloseHandle(hEvent);

    return status;
}
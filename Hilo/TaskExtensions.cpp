#include "pch.h"

namespace Hilo 
{
    // The run_async_non_interactive function allows lower priority (noninteractive) work to be scheduled on the main thread.
    // <snippet412>
    void run_async_non_interactive(std::function<void ()>&& action)
    {
        Windows::UI::Core::CoreWindow^ wnd = Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow;
        assert(wnd != nullptr);

        wnd->Dispatcher->RunAsync(
            Windows::UI::Core::CoreDispatcherPriority::Low, 
            ref new Windows::UI::Core::DispatchedHandler([action]()
        {
            action();
        })); 
    } 
    // </snippet412>
}

#ifndef NDEBUG
unsigned int _mainThreadId = 0U;

#ifdef  __cplusplus
extern "C" {
#endif

    // The IsMainThread function returns true if the current thread is the app's main thread and false otherwise.
    bool IsMainThread()
    {
        return (_mainThreadId == 0U || _mainThreadId == GetCurrentThreadId());
    }

    // The IsBackgroundThread function returns false if the current thread is the app's main thread and true otherwise.
    bool IsBackgroundThread()
    {
        return (_mainThreadId == 0U || _mainThreadId != GetCurrentThreadId());
    }

    // The RecordMainThread function registers the main thread ID for use by the IsMainThread and IsBackgroundThread functions.
    void RecordMainThread()
    {
        _mainThreadId = GetCurrentThreadId();
    }

#ifdef  __cplusplus
}
#endif

#endif /* not NDEBUG */
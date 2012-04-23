//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once
#include "CppUnitTest.h"

namespace HiloTests
{
    class TestHelper
    {
    public:
        static void RunUISynced(std::function<void ()>&& action);

        template <typename T>
        static T RunSynced(concurrency::task<T> t, concurrency::task_status& status, bool pump = true)
        {
            HANDLE hEvent = CreateEventEx(NULL, NULL, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
            if (hEvent == NULL)
            {
                throw std::bad_alloc();
            }

            T result;
            status = concurrency::not_complete;
            t.then([&hEvent, &status, &result](concurrency::task<T> previousTask){
                try
                {
                    result = previousTask.get();
                    status = concurrency::completed;
               } 
                catch(...)
                {
                    status = concurrency::canceled;
                }
                SetEvent(hEvent);
            });

            if (pump)
            {
                DWORD dummy;
                CoWaitForMultipleHandles(0x8, INFINITE, 1, &hEvent, &dummy);
            }
            else
            {
                WaitForSingleObjectEx(hEvent, INFINITE,true);

            }
            CloseHandle(hEvent);


            return result;
        }

        static void RunSynced(concurrency::task<void> t, concurrency::task_status& status, bool pump = true)
        {
            HANDLE hEvent = CreateEventEx(NULL, NULL, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
            if (hEvent == NULL)
            {
                throw std::bad_alloc();
            }

            status = concurrency::not_complete;
            t.then([&hEvent, &status](concurrency::task<void> previousTask){
                try
                {
                    previousTask.get();
                    status = concurrency::completed;
               } 
                catch(...)
                {
                    status = concurrency::canceled;
                }
                SetEvent(hEvent);
            });

            if (pump)
            {
                DWORD dummy;
                CoWaitForMultipleHandles(0x8, INFINITE, 1, &hEvent, &dummy);
            }
            else
            {
                WaitForSingleObjectEx(hEvent, INFINITE,true);

            }
            CloseHandle(hEvent);

            return;
        }

    private:
        template <typename T>
        static concurrency::task_status TaskWait(concurrency::task<T>& t, bool pump = true);
    };
};
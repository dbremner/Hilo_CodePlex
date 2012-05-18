﻿//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "CppUnitTest.h"
#include "StubExceptionPolicy.h"
#include "..\Hilo\TaskExceptionsExtensions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Hilo;

namespace HiloTests
{
    TEST_CLASS(TaskExceptionTests)
    {
    public:
       TEST_METHOD(TaskExceptionTestsPolicyInvokedOnHandledException)
        {
            auto policy = ref new StubExceptionPolicy();
            concurrency::task_status status;

            TestHelper::RunSynced(concurrency::task<void>([]
            {
               throw ref new Platform::NotImplementedException();
            }).then(ObserveException<void>(policy)), status);

            Assert::AreEqual(concurrency::task_status::canceled, status);

            auto exception = dynamic_cast<Platform::NotImplementedException^>(policy->SuppliedException);
            Assert::IsNotNull(exception);
        }

        TEST_METHOD(TaskExceptionTestsPolicyNotInvokedOnCancelledTask)
        {
            auto policy = ref new StubExceptionPolicy();
            concurrency::task_status status;

            TestHelper::RunSynced(concurrency::task<void>([]
            {
               concurrency::cancel_current_task();

            }).then(ObserveException<void>(policy)), status);

            Assert::AreEqual(concurrency::task_status::canceled, status);

            Assert::IsNull(policy->SuppliedException);
        }

        TEST_METHOD(TaskExceptionTestsPolicyInvokedWhenStandardExceptionRaised)
        {
            auto policy = ref new StubExceptionPolicy();
            concurrency::task_status status;

            TestHelper::RunSynced(concurrency::task<void>([]
            {
               throw std::exception("test");

            }).then(ObserveException<void>(policy)), status);

            Assert::AreEqual(concurrency::task_status::canceled, status);

            auto exception = dynamic_cast<Platform::FailureException^>(policy->SuppliedException);
            Assert::IsNotNull(exception);
        }

        TEST_METHOD(TaskExceptionTestsPolicyNotInvokedWithNoException)
        {
            auto policy = ref new StubExceptionPolicy();
            concurrency::task_status status;

           TestHelper::RunSynced(concurrency::task<void>([]
            {
               return;
            }).then(ObserveException<void>(policy)), status);

            Assert::AreEqual(concurrency::task_status::completed, status);

            Assert::IsNull(policy->SuppliedException);
        }
    };
}
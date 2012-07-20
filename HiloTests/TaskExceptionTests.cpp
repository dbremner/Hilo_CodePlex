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
            auto policy = std::make_shared<StubExceptionPolicy>();
            concurrency::task_status status;

            TestHelper::RunSynced(concurrency::task<void>([]
            {
                throw ref new Platform::NotImplementedException();
            }).then(ObserveException<void>(policy)), status);

            Assert::AreEqual(concurrency::task_status::canceled, status);

            auto exception = dynamic_cast<Platform::NotImplementedException^>(policy->GetSuppliedException());
            Assert::IsNotNull(exception);
        }

        TEST_METHOD(TaskExceptionTestsPolicyNotInvokedOnCancelledTask)
        {
            auto policy = std::make_shared<StubExceptionPolicy>();
            concurrency::task_status status;

            TestHelper::RunSynced(concurrency::task<void>([]
            {
                concurrency::cancel_current_task();

            }).then(ObserveException<void>(policy)), status);

            Assert::AreEqual(concurrency::task_status::canceled, status);

            Assert::IsNull(policy->GetSuppliedException());
        }

        TEST_METHOD(TaskExceptionTestsPolicyInvokedWhenStandardExceptionRaised)
        {
            auto policy = std::make_shared<StubExceptionPolicy>();
            concurrency::task_status status;

            TestHelper::RunSynced(concurrency::task<void>([]
            {
                throw std::exception("test");

            }).then(ObserveException<void>(policy)), status);

            Assert::AreEqual(concurrency::task_status::canceled, status);

            auto exception = dynamic_cast<Platform::FailureException^>(policy->GetSuppliedException());
            Assert::IsNotNull(exception);
        }

        TEST_METHOD(TaskExceptionTestsPolicyNotInvokedWithNoException)
        {
            auto policy = std::make_shared<StubExceptionPolicy>();
            concurrency::task_status status;

            TestHelper::RunSynced(concurrency::task<void>([]
            {
                return;
            }).then(ObserveException<void>(policy)), status);

            Assert::AreEqual(concurrency::task_status::completed, status);

            Assert::IsNull(policy->GetSuppliedException());
        }

        TEST_METHOD(TaskExceptionTestsPolicyNotInvokedWhenTaskCancelled)
        {
            auto policy = std::make_shared<StubExceptionPolicy>();
            concurrency::task_status status;

            TestHelper::RunSynced(concurrency::task<void>([]
            {
                concurrency::cancel_current_task();
            }).then(ObserveException<void>(policy)), status);

            Assert::AreEqual(concurrency::task_status::canceled, status);

            Assert::IsNull(policy->GetSuppliedException());
        }

        TEST_METHOD(TaskExceptionTestsPolicyNotInvokedWhenNonVoidTaskCancelled)
        {
            auto policy = std::make_shared<StubExceptionPolicy>();
            concurrency::task_status status;
            TestHelper::RunSynced(concurrency::task<int>([]
            {
                concurrency::cancel_current_task();
                return 3; // Should never reach here
            }).then(ObserveException<int>(policy)), status);

            Assert::AreEqual(concurrency::task_status::canceled, status);

            Assert::IsNull(policy->GetSuppliedException());
        }

        TEST_METHOD(TaskExceptionTestsValueBasedContinuationsNotExecutedAfterTaskCanceledAndObserved)
        {
            auto policy = std::make_shared<StubExceptionPolicy>();
            concurrency::task_status status;
            bool reachedContinuation = false;

            TestHelper::RunSynced(concurrency::task<int>([]
            {
                concurrency::cancel_current_task();

                return 3;  // should never reach here
            }).then(ObserveException<int>(policy))
                .then([&reachedContinuation](int value) 
            {
                reachedContinuation = true;
            }), status);

            Assert::AreEqual(concurrency::task_status::canceled, status);

            Assert::IsFalse(reachedContinuation);
        }
    };
}
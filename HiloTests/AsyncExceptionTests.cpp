//===============================================================================
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
#include "..\Hilo\AsyncException.h"
#include "..\Hilo\IExceptionPolicy.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Hilo;

namespace HiloTests
{
    TEST_CLASS(AsyncExceptionTests)
    {
    public:
        TEST_METHOD(AsyncExceptionTestsPolicyInvokedOnHandledException)
        {

            auto policy = ref new StubExceptionPolicy();
            concurrency::task_status status;

            TestHelper::RunSynced(concurrency::task<void>([policy]
            {
                Windows::Foundation::IAsyncAction^ action = concurrency::create_async([]{ throw ref new Platform::NotImplementedException(); });

                return AsyncException::ObserveWithPolicy(policy, action);
            }), status);

            Assert::AreEqual(concurrency::task_status::canceled, status);

            auto exception = dynamic_cast<Platform::NotImplementedException^>(policy->SuppliedException);
            Assert::IsNotNull(exception);
        }

        TEST_METHOD(AsyncExceptionTestsPolicyNotInvokedWithNoException)
        {
            auto policy = ref new StubExceptionPolicy();
            concurrency::task_status status;

            TestHelper::RunSynced(concurrency::task<void>([policy]
            {
                Windows::Foundation::IAsyncAction^ action = concurrency::create_async([]{ return; });

                return AsyncException::ObserveWithPolicy(policy, action);
            }), status);

            Assert::AreEqual(concurrency::task_status::completed, status);

            Assert::IsNull(policy->SuppliedException);
        }
    };
}
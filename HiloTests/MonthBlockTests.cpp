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
#include "..\Hilo\MonthBlock.h"
#include "StubExceptionPolicy.h"
#include "StubYearGroup.h"
#include "StubResourceLoader.h"
#include "StubPhoto.h"
#include "StubMonthBlockQuery.h"

using namespace concurrency;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Hilo;
using namespace Platform;

namespace HiloTests
{
    TEST_CLASS(MonthBlockTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_yearGroup = ref new StubYearGroup("2000", 2000);
            m_resourceLoader = ref new StubResourceLoader();
            m_query = std::make_shared<StubMonthBlockQuery>();
            m_exceptionPolicy = std::make_shared<StubExceptionPolicy>();
        }

        TEST_METHOD(MonthBlockShouldGetTitleForMonthFromResourceLoader)
        {
            StubResourceLoader^ loader = dynamic_cast<StubResourceLoader^>(m_resourceLoader);
            loader->StringToReturn = "Dec";
            auto model = ref new MonthBlock(m_yearGroup, 12, m_resourceLoader, m_query, m_exceptionPolicy);

            auto name = model->Name;

            Assert::AreEqual("Dec", name);
        }

        TEST_METHOD(MonthBlockShouldSetMonthFromConstructor)
        {
            unsigned int expected = 12U;
            auto model = ref new MonthBlock(m_yearGroup, expected, m_resourceLoader, m_query, m_exceptionPolicy);

            auto month = model->Month;

            Assert::AreEqual(expected, month);
        }

        TEST_METHOD(MonthBlockShouldCallPhotoCountQueryOnRepository)
        {
            auto model = ref new MonthBlock(m_yearGroup, 12, m_resourceLoader, m_query, m_exceptionPolicy);
            task_status status;

            TestHelper::RunSynced(model->QueryPhotoCount(), status);

            Assert::AreEqual(completed, status);
            Assert::IsTrue(m_query->GetHasBeenCalled());
        }

        TEST_METHOD(MonthBlockShouldSetQueryWhenGettingPhotoCount)
        {
            auto model = ref new MonthBlock(m_yearGroup, 12, m_resourceLoader, m_query, m_exceptionPolicy);
            task_status status;

            TestHelper::RunSynced(model->QueryPhotoCount(), status);

            Assert::AreEqual(completed, status);
            Assert::IsNotNull(m_query->GetDateRangeQuery());
        }

        TEST_METHOD(MonthBlockShouldHaveItemsWhenItemsReturnedFromQuery)
        {
            auto model = ref new MonthBlock(m_yearGroup, 12, m_resourceLoader, m_query, m_exceptionPolicy);
            task_status status;

            TestHelper::RunSynced(model->QueryPhotoCount(), status);

            Assert::AreEqual(completed, status);
            Assert::IsTrue(model->HasPhotos);
        }

    private:
        StubYearGroup^ m_yearGroup;
        StubResourceLoader^ m_resourceLoader;
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
        std::shared_ptr<StubMonthBlockQuery> m_query;
    };
}
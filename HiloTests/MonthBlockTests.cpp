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
#include "..\Hilo\IExceptionPolicy.h"
#include "..\Hilo\IQueryOperation.h"
#include "..\Hilo\IRepository.h"
#include "..\Hilo\IMonthBlock.h"
#include "..\Hilo\IYearGroup.h"
#include "..\Hilo\IResourceLoader.h"
#include "StubRepository.h"
#include "StubQueryOperation.h"
#include "StubExceptionPolicy.h"
#include "StubYearGroup.h"
#include "StubResourceLoader.h"
#include "StubPhoto.h"

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
            m_repository = ref new StubRepository();
            m_exceptionPolicy = ref new StubExceptionPolicy();
            m_queryOperation = ref new StubQueryOperation(nullptr);
            m_yearGroup = ref new StubYearGroup("2000", 2000);
            m_resourceLoader = ref new StubResourceLoader();
        }

        TEST_METHOD(MonthBlockShouldGetTitleForMonthFromResourceLoader)
        {
            StubResourceLoader^ loader = dynamic_cast<StubResourceLoader^>(m_resourceLoader);
            loader->StringToReturn = "Dec";
            auto model = ref new MonthBlock(m_yearGroup, 12, m_resourceLoader, m_repository, m_queryOperation, m_exceptionPolicy);

            auto name = model->Name;

            Assert::AreEqual("Dec", name);
        }

        TEST_METHOD(MonthBlockShouldSetMonthFromConstructor)
        {
            unsigned int expected = 12U;
            auto model = ref new MonthBlock(m_yearGroup, expected, m_resourceLoader, m_repository, m_queryOperation, m_exceptionPolicy);

            auto month = model->Month;

            Assert::AreEqual(expected, month);
        }

        TEST_METHOD(MonthBlockShouldCallPhotoCountQueryOnRepository)
        {
            auto model = ref new MonthBlock(m_yearGroup, 12, m_resourceLoader, m_repository, m_queryOperation, m_exceptionPolicy);
            task_status status;
            StubRepository^ repository = dynamic_cast<StubRepository^>(m_repository);
            repository->PhotoToReturn = ref new StubPhoto();

            TestHelper::RunSynced(model->QueryPhotoCount(), status);

            Assert::AreEqual(completed, status);
            Assert::IsTrue(repository->GetPhotoCountForQueryOperationAsyncCalled);
        }

        TEST_METHOD(MonthBlockShouldSetQueryWhenGettingPhotoCount)
        {
            auto model = ref new MonthBlock(m_yearGroup, 12, m_resourceLoader, m_repository, m_queryOperation, m_exceptionPolicy);
            task_status status;
            StubRepository^ repository = dynamic_cast<StubRepository^>(m_repository);
            repository->PhotoToReturn = ref new StubPhoto();

            TestHelper::RunSynced(model->QueryPhotoCount(), status);

            Assert::AreEqual(completed, status);
            Assert::IsNotNull(m_queryOperation->Query);
        }

        TEST_METHOD(MonthBlockShouldHaveItemsWhenItemsReturnedFromQuery)
        {
            auto model = ref new MonthBlock(m_yearGroup, 12, m_resourceLoader, m_repository, m_queryOperation, m_exceptionPolicy);
            task_status status;
            StubRepository^ repository = dynamic_cast<StubRepository^>(m_repository);
            repository->PhotoToReturn = ref new StubPhoto();

            TestHelper::RunSynced(model->QueryPhotoCount(), status);

            Assert::AreEqual(completed, status);
            Assert::IsTrue(model->HasPhotos);
        }

    private:
        IExceptionPolicy^ m_exceptionPolicy;
        IRepository^ m_repository;
        IQueryOperation^ m_queryOperation;
        IYearGroup^ m_yearGroup;
        IResourceLoader^ m_resourceLoader;
    };
}
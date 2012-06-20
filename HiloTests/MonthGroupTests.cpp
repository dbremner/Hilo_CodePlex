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
#include "..\Hilo\MonthGroup.h"
#include "..\Hilo\IExceptionPolicy.h"
#include "..\Hilo\IQueryOperation.h"
#include "..\Hilo\IRepository.h"
#include "..\Hilo\IPhotoCache.h"
#include "StubRepository.h"
#include "StubQueryOperation.h"
#include "StubExceptionPolicy.h"
#include "StubPhoto.h"
#include "StubPhotoCache.h"

using namespace concurrency;
using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;

namespace HiloTests
{
    TEST_CLASS(MonthGroupTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_repository = ref new StubRepository();
            m_exceptionPolicy = ref new StubExceptionPolicy();
            m_queryOperation = ref new StubQueryOperation(nullptr);
            m_photoCache = ref new StubPhotoCache();
        }

        TEST_METHOD(MonthGroupSubscibesToDataChangeEventOnConstruction)
        {
            StubRepository^ repository = dynamic_cast<StubRepository^>(m_repository);

            auto group = ref new MonthGroup("Title", m_photoCache, m_repository, m_queryOperation, m_exceptionPolicy);

            Assert::IsTrue(repository->DataChangedEventObserved);
        }

        TEST_METHOD(MonthGroupReturnsTitleAsNamePlusCount)
        {
            StubRepository^ repository = dynamic_cast<StubRepository^>(m_repository);
            repository->PhotoToReturn = ref new StubPhoto();
            auto group = ref new MonthGroup("Title", m_photoCache, m_repository, m_queryOperation, m_exceptionPolicy);
            String^ expected = "Title (1)";
            task_status status;
            TestHelper::RunSynced(group->QueryPhotosAsync(), status);

            auto title = group->Title;

            Assert::AreEqual(expected, title);
        }

        TEST_METHOD(MonthGroupCallsGetPhotosForQueryOnRepositoryWhenQueryPhotoAsyncCalled)
        {
            StubRepository^ repository = dynamic_cast<StubRepository^>(m_repository);
            auto group = ref new MonthGroup("Title", m_photoCache, m_repository, m_queryOperation, m_exceptionPolicy);
            task_status status;
            
            TestHelper::RunSynced(group->QueryPhotosAsync(), status);

            Assert::IsTrue(repository->GetPhotoGroupDataForGroupWithQueryOperationAsyncCalled);
        }

    private:
        IExceptionPolicy^ m_exceptionPolicy;
        IRepository^ m_repository;
        IQueryOperation^ m_queryOperation;
        IPhotoCache^ m_photoCache;
    };
}
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
#include "..\Hilo\PhotoCache.h"
#include "StubExceptionPolicy.h"
#include "StubPhoto.h"
#include "StubMonthGroupQuery.h"

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
            m_exceptionPolicy = std::make_shared<StubExceptionPolicy>();
            m_query = std::make_shared<StubMonthGroupQuery>();
            m_photoCache = std::make_shared<PhotoCache>();
        }

        TEST_METHOD(MonthGroupReturnsTitleAsNamePlusCount)
        {
            m_query->SetPhotoToReturn(ref new StubPhoto());
            auto group = ref new MonthGroup("Title", m_photoCache, m_query, m_exceptionPolicy);
            String^ expected = "Title (1)";
            task_status status;
            TestHelper::RunSynced(group->QueryPhotosAsync(), status);

            auto title = group->Title;

            Assert::AreEqual(expected, title);
        }

        TEST_METHOD(MonthGroupCallsGetPhotosForQueryOnRepositoryWhenQueryPhotoAsyncCalled)
        {
            auto group = ref new MonthGroup("Title", m_photoCache, m_query, m_exceptionPolicy);
            task_status status;
            
            TestHelper::RunSynced(group->QueryPhotosAsync(), status);

            Assert::IsTrue(m_query->GetHasBeenCalled());
        }

    private:
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
        std::shared_ptr<PhotoCache> m_photoCache;
        std::shared_ptr<StubMonthGroupQuery> m_query;
    };
}
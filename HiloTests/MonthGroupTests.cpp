#include "pch.h"
#include "CppUnitTest.h"
#include "..\Hilo\MonthGroup.h"
#include "..\Hilo\PhotoCache.h"
#include "StubExceptionPolicy.h"
#include "StubPhoto.h"
#include "StubRepository.h"

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
            m_repository = std::make_shared<StubRepository>(m_exceptionPolicy);
            m_photoCache = std::make_shared<PhotoCache>();
        }

        TEST_METHOD(MonthGroupHasPhotosPropertyReturnsFalse)
        {
            auto group = ref new MonthGroup(m_photoCache, nullptr, m_repository, m_exceptionPolicy);
            Assert::IsFalse(group->HasPhotos);
        }

        TEST_METHOD(MonthGroupWhenConstructedReturnsFalseForRunningProperty)
        {
            auto group = ref new MonthGroup(m_photoCache, nullptr, m_repository, m_exceptionPolicy);
            Assert::IsFalse(group->IsRunning);
        }

        TEST_METHOD(MonthGroupWhenConstructedReturnsNullTitle)
        {
            m_repository->SetPhotoToReturn(ref new StubPhoto());
            auto group = ref new MonthGroup(m_photoCache, nullptr, m_repository, m_exceptionPolicy);
            Assert::IsNull(group->Title);
        }

        TEST_METHOD(MonthGroupCallsGetPhotosForQueryOnRepositoryWhenQueryPhotoAsyncCalled)
        {
            auto group = ref new MonthGroup(m_photoCache, nullptr, m_repository, m_exceptionPolicy);
            task_status status;
            TestHelper::RunSynced(group->QueryPhotosAsync(), status);
            Assert::IsTrue(m_repository->GetPhotoDataForMonthGroupHasBeenCalled());
        }

    private:
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
        std::shared_ptr<PhotoCache> m_photoCache;
        std::shared_ptr<StubRepository> m_repository;
    };
}
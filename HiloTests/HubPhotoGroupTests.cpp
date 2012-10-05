#include "pch.h"
#include "CppUnitTest.h"
#include "UnitTestingAssertSpecializations.h"
#include "..\Hilo\HubPhotoGroup.h"
#include "StubExceptionPolicy.h"
#include "StubPhoto.h"
#include "StubRepository.h"

using namespace concurrency;
using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HiloTests
{
    TEST_CLASS(HubPhotoGroupTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_exceptionPolicy = std::make_shared<StubExceptionPolicy>();
            m_repository = std::make_shared<StubRepository>(m_exceptionPolicy);
        }

        TEST_METHOD(HubPhotoGroupShouldCallRepositoryToGetPhotos)
        {
            auto photoGroup = ref new HubPhotoGroup("Title", "No Title", m_repository, m_exceptionPolicy);
            task_status status;
            
            TestHelper::RunUISynced([photoGroup, &status]
            {
                TestHelper::RunSynced(photoGroup->QueryPhotosAsync(), status);
            });

            Assert::AreEqual(completed, status);
            Assert::IsTrue(m_repository->GetPhotosForPictureHubGroupAsyncHasBeenCalled());
        }

        TEST_METHOD(HubPhotoGroupShouldAddItselfAsObserverOfQuery)
        {
            auto photoGroup = ref new HubPhotoGroup("Title", "No Title", m_repository, m_exceptionPolicy);      
            Assert::IsTrue(m_repository->GetAddObserverHasBeenCalled());
        }

        TEST_METHOD(HubPhotoGroupShouldReturnEmptyTitleWhenNoPicturesArePresent)
        {
            auto photoGroup = ref new HubPhotoGroup("Title", "No Title", m_repository, m_exceptionPolicy);
            task_status status;

            TestHelper::RunUISynced([photoGroup, &status]
            {
                TestHelper::RunSynced(photoGroup->QueryPhotosAsync(), status);
            });

            Assert::AreEqual(completed, status);
            Assert::AreEqual("No Title", photoGroup->Title);
        }

        TEST_METHOD(HubPhotoGroupShouldReturnTitleWhenPicturesArePresent)
        {
            auto photoGroup = ref new HubPhotoGroup("Title", "No Title", m_repository, m_exceptionPolicy);
            task_status status;
            m_repository->SetPhotoToReturn(ref new StubPhoto());

            TestHelper::RunUISynced([photoGroup, &status]
            {
                TestHelper::RunSynced(photoGroup->QueryPhotosAsync(), status);
            });

            Assert::AreEqual(completed, status);
            Assert::AreEqual("Title", photoGroup->Title);
        }

    private:
        std::shared_ptr<StubRepository> m_repository;
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
    };
}
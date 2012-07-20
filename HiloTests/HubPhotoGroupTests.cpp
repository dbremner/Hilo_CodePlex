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
#include "..\Hilo\HubPhotoGroup.h"
#include "StubExceptionPolicy.h"
#include "StubPhoto.h"
#include "StubPictureHubGroupQuery.h"

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
            m_query = std::make_shared<StubPictureHubGroupQuery>();
            m_exceptionPolicy = std::make_shared<StubExceptionPolicy>();
        }

        TEST_METHOD(HubPhotoGroupShouldCallRepositoryToGetPhotos)
        {
            auto photoGroup = ref new HubPhotoGroup("Title", "No Title", m_query, m_exceptionPolicy);
            task_status status;
            
            TestHelper::RunUISynced([photoGroup, &status]
            {
                TestHelper::RunSynced(photoGroup->QueryPhotosAsync(), status);
            });

            Assert::AreEqual(completed, status);
            Assert::IsTrue(m_query->GetHasBeenCalled());
        }

        TEST_METHOD(HubPhotoGroupShouldAddItselfAsObserverOfQuery)
        {
            auto photoGroup = ref new HubPhotoGroup("Title", "No Title", m_query, m_exceptionPolicy);      
            Assert::IsTrue(m_query->GetObserved());
        }

        TEST_METHOD(HubPhotoGroupShouldReturnEmptyTitleWhenNoPicturesArePresent)
        {
            auto photoGroup = ref new HubPhotoGroup("Title", "No Title", m_query, m_exceptionPolicy);
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
            auto photoGroup = ref new HubPhotoGroup("Title", "No Title", m_query, m_exceptionPolicy);
            task_status status;
            m_query->SetPhotoToReturn(ref new StubPhoto());

            TestHelper::RunUISynced([photoGroup, &status]
            {
                TestHelper::RunSynced(photoGroup->QueryPhotosAsync(), status);
            });

            Assert::AreEqual(completed, status);
            Assert::AreEqual("Title", photoGroup->Title);
        }

    private:
        std::shared_ptr<StubPictureHubGroupQuery> m_query;
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
    };
}
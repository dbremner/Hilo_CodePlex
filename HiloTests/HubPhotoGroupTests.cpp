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
#include "..\Hilo\IPhoto.h"
#include "..\Hilo\IRepository.h"
#include "..\Hilo\HubPhotoGroup.h"
#include "StubExceptionPolicy.h"
#include "StubPhoto.h"
#include "StubRepository.h"

using namespace concurrency;
using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::BulkAccess;

namespace HiloTests
{
    TEST_CLASS(HubPhotoGroupTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_repository = ref new StubRepository();
            m_exceptionPolicy = ref new StubExceptionPolicy();
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
            Assert::IsTrue(m_repository->GetPhotosForGroupWithQueryOperationAsyncCalled);
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
            m_repository->PhotoToReturn = ref new StubPhoto();

            TestHelper::RunUISynced([photoGroup, &status]
            {
                TestHelper::RunSynced(photoGroup->QueryPhotosAsync(), status);
            });

            Assert::AreEqual(completed, status);
            Assert::AreEqual("Title", photoGroup->Title);
        }

    private:
        StubRepository^ m_repository;
        StubExceptionPolicy^ m_exceptionPolicy;
    };
}
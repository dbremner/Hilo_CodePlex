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
#include "..\Hilo\PhotoQueryBuilder.h"

using namespace concurrency;
using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::BulkAccess;

namespace HiloTests
{
    TEST_CLASS(PhotoQueryBuilderTests)
    {
    public:

        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_imageGenerator = TestImageGenerator();
            concurrency::task_status status;
            auto images = TestHelper::RunSynced(
                m_imageGenerator.CreateTestImagesFromLocalFolder("UnitTestLogo.png", 10, "random_test"), 
                status);
            Assert::AreEqual(concurrency::task_status::completed, status);
        }

        TEST_METHOD_CLEANUP(Cleanup)
        {
            concurrency::task_status status;
            TestHelper::RunSynced(m_imageGenerator.DeleteFilesAsync(), status);
        }

        TEST_METHOD(PhotoQueryBuilderShouldReadNoMoreThanMaxNumberOfPhotos)
        {
            PhotoQueryBuilder query;
            unsigned int maxNumberOfItems = 5;
            auto result = query.GetPhotosAsync("", maxNumberOfItems);
            task_status status;
            
            auto photos = TestHelper::RunSynced(result.GetStorageItemsTask(), status);

            Assert::AreEqual(completed, status);
            Assert::IsTrue(photos->Size <= maxNumberOfItems);
        }
    private:
        TestImageGenerator m_imageGenerator;
    };
}
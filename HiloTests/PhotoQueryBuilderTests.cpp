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

namespace HiloTests
{
    TEST_CLASS(PhotoQueryBuilderTests)
    {
    public:

        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_imageGenerator = TestImageGenerator();
            concurrency::task_status status;
            m_images = TestHelper::RunSynced(
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
            auto result = query.GetPhotosAsync("", cancellation_token::none(), maxNumberOfItems);
            task_status status;
            
            auto photos = TestHelper::RunSynced(result.GetStorageItemsTask(), status);

            Assert::AreEqual(completed, status);
            Assert::IsTrue(photos->Size <= maxNumberOfItems);
        }

        TEST_METHOD(PhotoQueryBuilderShouldReturnSingleValidPhoto)
        {
            auto image = m_images->GetAt(0);
            
            PhotoQueryBuilder query ;
            auto result = query.GetPhotoAsync(image->Path, cancellation_token::none());
            task_status status;

            auto photo = TestHelper::RunSynced(result.GetStorageItemsTask(), status);
            Assert::AreEqual(completed, status);

            Assert::AreEqual(photo->Size, 1U);
        }

    private:
        TestImageGenerator m_imageGenerator;
        IVectorView<Windows::Storage::StorageFile^>^ m_images;
    };
}
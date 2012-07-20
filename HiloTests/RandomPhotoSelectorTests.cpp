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
#include "..\Hilo\RandomPhotoSelector.h"
#include "..\Hilo\PhotoQueryBuilder.h"
#include <memory>

using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;

namespace HiloTests
{
    TEST_CLASS(RandomPhotoSelectorTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_imageGenerator = TestImageGenerator();
        }

        TEST_METHOD_CLEANUP(Cleanup)
        {
            concurrency::task_status status;
            TestHelper::RunSynced(m_imageGenerator.DeleteFilesAsync(), status);
        }

        TEST_METHOD(RandomPhotoSelectorTestsSelectsFivePhotos)
        {
            concurrency::task_status status;

            auto selectedPhotos = TestHelper::RunSynced(
                m_imageGenerator.CreateTestImagesFromLocalFolder("UnitTestLogo.png", 10, "random_test")
                .then([this](IVectorView<StorageFile^>^ photos)
            { 
                auto copied = ref new Vector<StorageFile^>(begin(photos), end(photos));
                return RandomPhotoSelector::SelectFilesAsync(copied->GetView(), 5);
            }), status);

            Assert::AreEqual(concurrency::completed, status);
            Assert::AreEqual(5U, selectedPhotos->Size);		
        }

        TEST_METHOD(RandomPhotoSelectorUsesSizeOfProvidedFilesIfLessThanRequestedCount)
        {
            concurrency::task_status status;

            auto selectedPhotos = TestHelper::RunSynced(
                m_imageGenerator.CreateTestImagesFromLocalFolder("UnitTestLogo.png", 10, "random_test")
                .then([](IVectorView<StorageFile^>^ photos)
            { 
                auto copied = ref new Vector<StorageFile^>(begin(photos), end(photos));
                return RandomPhotoSelector::SelectFilesAsync(copied->GetView(), 15);
            }), status);

            Assert::AreEqual(concurrency::completed, status);
            Assert::AreEqual(10U, selectedPhotos->Size);
        }

    private:
        TestImageGenerator m_imageGenerator;
    };
}
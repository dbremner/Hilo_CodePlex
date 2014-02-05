// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "CppUnitTest.h"
#include "UnitTestingAssertSpecializations.h"
#include "..\Hilo\RandomPhotoSelector.h"

using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;

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
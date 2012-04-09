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
#include "..\Hilo\PhotoReader.h"
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

        TEST_METHOD(RandomPhotoSelectorTestsSelectsFivePhotos)
        {
            concurrency::task_status status;
            auto photos = TestHelper::RunAsync(
                TestHelper::CreateTestImagesFromLocalFolder("UnitTestLogo.png", 10, "random_test"),
                status);

            Assert::AreEqual(concurrency::completed, status);   

            auto selectedPhotos = TestHelper::RunAsync(
                TestHelper::CreateTestImagesFromLocalFolder("UnitTestLogo.png", 10, "random_test")
                .then([](IVectorView<StorageFile^>^ photos)
                { 
                    // workaround for threading issue
                    
                    auto copied = ref new Vector<StorageFile^>(begin(photos), end(photos));
                    return RandomPhotoSelector::SelectFilesAsync(copied->GetView(), 5);
                }), status);

            Assert::AreEqual(concurrency::completed, status);

            Assert::AreEqual(5U, selectedPhotos->Size);
        }
    };
}
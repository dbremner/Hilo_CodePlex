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
#include "..\Hilo\PhotoReader.h"

using namespace Hilo;

using namespace concurrency;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::BulkAccess;

namespace HiloTests
{
    TEST_CLASS(PhotoReaderTests)
    {
    public:
        TEST_METHOD(PhotoReaderShouldReadNoMoreThanMaxNumberOfPhotos)
        {
            PhotoReader reader;
            unsigned int maxNumberOfItems = 5;
            task<IVectorView<FileInformation^>^> t = reader.GetPhotoGroupAsync("", maxNumberOfItems);
            task_status status;            
            
            auto result = TestHelper::RunAsync(t, status);

            Assert::AreEqual(completed, status);
            Assert::IsTrue(result->Size <= maxNumberOfItems);
        }

        TEST_METHOD(PhotoReaderShouldGetVirtualizedFileList)
        {
            concurrency::task_status status;
            auto photos = TestHelper::RunAsync(
                TestHelper::CreateTestImagesFromLocalFolder("UnitTestLogo.png", 1, "random_test"),
                status);

            Assert::AreEqual(concurrency::completed, status); 
        }
    };
}
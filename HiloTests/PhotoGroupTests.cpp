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
#include "..\Hilo\PhotoGroup.h"
#include "..\Hilo\Photo.h"

using namespace Hilo;

using namespace concurrency;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::BulkAccess;

namespace HiloTests
{
    TEST_CLASS(PhotoGroupTests)
    {
    public:
        TEST_METHOD(PhotoGroupShouldBeAbleToSetTitleForGroup)
        {
            PhotoGroup^ photoGroup = ref new PhotoGroup(GetPhotosAsync());

            photoGroup->Title = "Test";

            Assert::AreEqual("Test", photoGroup->Title);
        }

        TEST_METHOD(PhotoGroupShouldCallFunctionToGetPhotos)
        {
            PhotoGroup^ photoGroup = ref new PhotoGroup(GetPhotosAsync());
            task<Object^> itemsTask([&photoGroup]()-> Object^ {
                return photoGroup->Items;
            });
            task_status status;

            Object^ photos = TestHelper::RunAsync<Object^>(itemsTask, status);

            Assert::AreEqual(completed, status);
            Assert::IsNotNull(photos);
        }

        IAsyncOperation<IVectorView<FileInformation^>^>^ GetPhotosAsync()
        {
            concurrency::task_status status;
            FileInformation^ file = TestHelper::RunAsync<FileInformation^>(TestHelper::CreateTestImageFileFromLocalFolder("UnitTestLogo.png"), status);

            task<IVectorView<FileInformation^>^> task([file]()-> IVectorView<FileInformation^>^ {
                Vector<FileInformation^>^ files = ref new Vector<FileInformation^>();
                files->Append(file);
                return files->GetView();
            });

            return create_async([task]() { 
                return task;
            });
        }
    };
}
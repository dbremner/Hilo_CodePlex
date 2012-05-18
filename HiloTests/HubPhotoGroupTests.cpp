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
    TEST_CLASS(HubPhotoGroupTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_generator = TestImageGenerator();
        }

        TEST_METHOD_CLEANUP(Cleanup)
        {
            concurrency::task_status status;
            TestHelper::RunSynced(m_generator.DeleteFilesAsync(), status);
        }

        TEST_METHOD(HubPhotoGroupShouldCallFunctionToGetPhotos)
        {
            bool photosNull = false;

            HubPhotoGroup^ photoGroup = ref new HubPhotoGroup("Test", "No Test", GetPhotosAsync());
            Object^ items;

            TestHelper::RunUISynced([this, photoGroup, &items]() {
                items = photoGroup->Items;
            });

            Assert::IsNotNull(items);
        }

        /*TEST_METHOD(HubPhotoGroupShouldReturnEmptyTitleWhenNoPicturesArePresent)
        {
            auto task = create_async([]() {
                auto empty = ref new Vector<FileInformation^>();
                return empty->GetView();
            });

            HubPhotoGroup^ photoGroup = ref new HubPhotoGroup("Test", "No Test", task);
            Object^ items;

            TestHelper::RunUISynced([this, photoGroup, &items]() {
                items = photoGroup->Items;
            });

            Assert::AreEqual("No Test", photoGroup->Title);
        }*/

        IAsyncOperation<IVectorView<FileInformation^>^>^ GetPhotosAsync()
        {
            concurrency::task_status status;
            FileInformation^ file = TestHelper::RunSynced<FileInformation^>(m_generator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png"), status);

            task<IVectorView<FileInformation^>^> task([file]()-> IVectorView<FileInformation^>^ {
                Vector<FileInformation^>^ files = ref new Vector<FileInformation^>();
                files->Append(file);
                return files->GetView();
            });

            return create_async([task]() { 
                return task;
            });
        }

    private:
        TestImageGenerator m_generator;
    };
}
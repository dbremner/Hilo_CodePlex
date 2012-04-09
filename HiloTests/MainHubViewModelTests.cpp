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
#include "..\Hilo\MainHubViewModel.h"

using namespace Hilo;

using namespace concurrency;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;

namespace HiloTests
{
    TEST_CLASS(MainHubViewModelTests)
    {
    public:
        TEST_METHOD(MainHubViewModelShouldGetPhotoGroupsForHub)
        {
            auto photoGroup = ref new PhotoGroup(GetPhotosAsync());
            photoGroup->Title = "Test";
            auto vector = ref new Vector<PhotoGroup^>();
            vector->Append(photoGroup);
            auto model = ref new MainHubViewModel(vector);
            task<Object^> photoGroupTasks([model]()-> Object^ {
                return model->PhotoGroups;
            });
            task_status status;

            Object^ photoGroups = TestHelper::RunAsync<Object^>(photoGroupTasks, status);

            Assert::AreEqual(completed, status);
            Assert::IsNotNull(photoGroups);
        }

        TEST_METHOD(MainHubViewModelShouldGetPhotoGroupsAsVectorOfPhotoGroups)
        {
            auto photoGroup = ref new PhotoGroup(GetPhotosAsync());
            photoGroup->Title = "Test";
            auto vector = ref new Vector<PhotoGroup^>();
            vector->Append(photoGroup);
            TypeName pageType;
            auto model = ref new MainHubViewModel(vector);
            task<Object^> photoGroupTasks([model]()-> Object^ {
                return model->PhotoGroups;
            });
            task_status status;

            Object^ photoGroupsAsObject = TestHelper::RunAsync<Object^>(photoGroupTasks, status);
            Vector<PhotoGroup^>^ photoGroups = dynamic_cast<Vector<PhotoGroup^>^>(photoGroupsAsObject);

            Assert::AreEqual(completed, status);
            Assert::IsNotNull(photoGroups);
            Assert::AreEqual(1U, photoGroups->Size);
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
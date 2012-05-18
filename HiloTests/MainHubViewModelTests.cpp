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
#include "..\Hilo\MainHubViewModel.h"
#include "..\Hilo\Photo.h"
#include "StubExceptionPolicy.h"
#include "StubPhotoGroup.h"

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
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_imageGenerator = TestImageGenerator();
        }

        TEST_METHOD_CLEANUP(Cleanup)
        {
            concurrency::task_status status;
            TestHelper::RunSynced(m_imageGenerator.DeleteFilesAsync(), status);
        }

        TEST_METHOD(MainHubViewModelShouldGetPhotoGroupsForHub)
        {
            auto photoGroup = ref new HubPhotoGroup("Test", "No Test", GetPhotosAsync());
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);
            auto model = ref new MainHubViewModel(vector, ref new StubExceptionPolicy());
            task<Object^> photoGroupTasks([model]()-> Object^ {
                return model->PhotoGroups;
            });
            task_status status;

            Object^ photoGroups = TestHelper::RunSynced<Object^>(photoGroupTasks, status);

            Assert::AreEqual(completed, status);
            Assert::IsNotNull(photoGroups);
        }

        TEST_METHOD(MainHubViewModelShouldGetPhotoGroupsAsIObservableVectorOfPhotoGroups)
        {
            auto photoGroup = ref new HubPhotoGroup("Test", "No Test", GetPhotosAsync());
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);
            TypeName pageType;
            auto model = ref new MainHubViewModel(vector, ref new StubExceptionPolicy());
            task<Object^> photoGroupTasks([model]()-> Object^ {
                return model->PhotoGroups;
            });
            task_status status;

            Object^ photoGroupsAsObject = TestHelper::RunSynced<Object^>(photoGroupTasks, status);
            IObservableVector<HubPhotoGroup^>^ photoGroups = dynamic_cast<IObservableVector<HubPhotoGroup^>^>(photoGroupsAsObject);

            Assert::AreEqual(completed, status);
            Assert::IsNotNull(photoGroups);
            Assert::AreEqual(1U, photoGroups->Size);
        }

        TEST_METHOD(MainHubViewModelShouldSetupNavigateCommandWhenConstructed)
        {
            auto photoGroup = ref new HubPhotoGroup("Test", "No Test", GetPhotosAsync());
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);

            MainHubViewModel model(vector, ref new StubExceptionPolicy());

            Assert::IsNotNull(model.NavigateToPicturesCommand);
        }

        TEST_METHOD(MainHubViewModelShouldSetupRotateCommandWhenConstructed)
        {
            auto photoGroup = ref new HubPhotoGroup("Test", "No Test", GetPhotosAsync());
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);

            MainHubViewModel model(vector, ref new StubExceptionPolicy());

            Assert::IsNotNull(model.RotateImageCommand);
        }

        TEST_METHOD(MainHubViewModelShouldSetupCropCommandWhenConstructed)
        {
            auto photoGroup = ref new HubPhotoGroup("Test", "No Test", GetPhotosAsync());
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);

            MainHubViewModel model(vector, ref new StubExceptionPolicy());

            Assert::IsNotNull(model.CropImageCommand);
        }

        TEST_METHOD(MainHubViewModelShouldDefaultToAppBarBeingDisabledWhenConstructed)
        {
            auto photoGroup = ref new HubPhotoGroup("Test", "No Test", GetPhotosAsync());
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);

            MainHubViewModel model(vector, ref new StubExceptionPolicy());

            Assert::IsFalse(model.IsAppBarEnabled);
        }

        TEST_METHOD(MainHubViewModelShouldDefaultToSelectedItemBeingNull)
        {
            auto photoGroup = ref new HubPhotoGroup("Test", "No Test", GetPhotosAsync());
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);

            MainHubViewModel model(vector, ref new StubExceptionPolicy());

            Assert::IsNull(model.SelectedItem);
        }

        TEST_METHOD(MainHubViewModelShouldEnableAppBarWhenSettingTheSelectedItemToAPhoto)
        {
            auto photoGroup = ref new HubPhotoGroup("Test", "No Test", GetPhotosAsync());
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);
            auto model = ref new MainHubViewModel(vector, ref new StubExceptionPolicy());
            auto t = task<IVectorView<FileInformation^>^>(GetPhotosAsync());
            task_status status;
            auto files = TestHelper::RunSynced<IVectorView<FileInformation^>^>(t, status);
            auto photo = ref new Photo(files->GetAt(0), ref new StubPhotoGroup(""));

            TestHelper::RunUISynced([model, photo] {
                model->SelectedItem = photo;
            });

            Assert::IsTrue(model->IsAppBarEnabled);
        }

        TEST_METHOD(MainHubViewModelShouldDisableAppBarWhenSettingTheSelectedItemToANullPtr)
        {
            auto photoGroup = ref new HubPhotoGroup("Test", "No Test", GetPhotosAsync());
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);
            auto model = ref new MainHubViewModel(vector, ref new StubExceptionPolicy());

            TestHelper::RunUISynced([model] {
                model->SelectedItem = nullptr;
            });

            Assert::IsFalse(model->IsAppBarEnabled);
        }

        TEST_METHOD(MainHubViewModelShouldFirePropertyChangeOfSelectedItemWhenSettingSelectedItem)
        {
            auto photoGroup = ref new HubPhotoGroup("Test", "No Test", GetPhotosAsync());
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);            
            auto model = ref new MainHubViewModel(vector, ref new StubExceptionPolicy());
            bool propertyChangedFired = false;
            model->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender,  PropertyChangedEventArgs^ e) 
            {
                if (e->PropertyName == "SelectedItem")
                {
                    propertyChangedFired = true;
                }
            });
             auto t = task<IVectorView<FileInformation^>^>(GetPhotosAsync());
            task_status status;
            auto files = TestHelper::RunSynced<IVectorView<FileInformation^>^>(t, status);
            auto photo = ref new Photo(files->GetAt(0), ref new StubPhotoGroup(""));

            TestHelper::RunUISynced([model, photo] {
                model->SelectedItem = photo;
            });

            Assert::IsTrue(propertyChangedFired);
        }

        TEST_METHOD(MainHubViewModelShouldFirePropertyChangeForIsAppBarEnabledWhenSettingSelectedItem)
        {
            auto photoGroup = ref new HubPhotoGroup("Test", "No Test", GetPhotosAsync());
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);            
            auto model = ref new MainHubViewModel(vector, ref new StubExceptionPolicy());
            bool propertyChangedFired = false;
            model->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender,  PropertyChangedEventArgs^ e) 
            {
                if (e->PropertyName == "IsAppBarEnabled")
                {
                    propertyChangedFired = true;
                }
            });

            TestHelper::RunUISynced([model] {
                model->IsAppBarEnabled = true;
            });

            Assert::IsTrue(propertyChangedFired);
        }



        IAsyncOperation<IVectorView<FileInformation^>^>^ GetPhotosAsync()
        {
            concurrency::task_status status;
            FileInformation^ file = TestHelper::RunSynced<FileInformation^>(m_imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png"), status);

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
        TestImageGenerator m_imageGenerator;
    };
}
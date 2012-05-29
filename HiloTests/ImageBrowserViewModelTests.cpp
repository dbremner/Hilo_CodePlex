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
#include "..\Hilo\ImageBrowserViewModel.h"
#include "..\Hilo\PhotoCache.h"
#include "..\Hilo\MonthGroup.h"
#include "..\Hilo\Photo.h"
#include "StubExceptionPolicy.h"
#include "StubPhotoGroup.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;

namespace HiloTests
{
    TEST_CLASS(ImageBrowserViewModelTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_imageGenerator = TestImageGenerator();
            m_exceptionPolicy = ref new StubExceptionPolicy();
            m_photoGroup = ref new StubPhotoGroup("");
        }

        TEST_METHOD_CLEANUP(Cleanup)
        {
            concurrency::task_status status;
            TestHelper::RunSynced(m_imageGenerator.DeleteFilesAsync(), status);
        }

        TEST_METHOD(ImageBrowserViewModelShouldSetupGroupCommandWhenConstructed)
        {
            ImageBrowserViewModel model(m_exceptionPolicy);
            Assert::IsNotNull(model.GroupCommand);
        }

        TEST_METHOD(ImageBrowserViewModelShouldSetupRotateCommandWhenConstructed)
        {
            ImageBrowserViewModel model(m_exceptionPolicy);
            Assert::IsNotNull(model.RotateImageCommand);
        }

        TEST_METHOD(ImageBrowserViewModelShouldSetupCropCommandWhenConstructed)
        {
            ImageBrowserViewModel model(m_exceptionPolicy);
            Assert::IsNotNull(model.CropImageCommand);
        }

        TEST_METHOD(ImageBrowserViewModelShouldDefaultToAppBarBeingDisabledWhenConstructed)
        {
            ImageBrowserViewModel model(m_exceptionPolicy);
            Assert::IsFalse(model.IsAppBarEnabled);
        }

        TEST_METHOD(ImageBrowserViewModelShouldDefaultToSelectedItemBeingNull)
        {
            ImageBrowserViewModel model(m_exceptionPolicy);
            Assert::IsNull(model.SelectedItem);
        }
       
        TEST_METHOD(ImageBrowserViewModelShouldEnableAppBarWhenSettingTheSelectedItemToAPhoto)
        {
            auto model = ref new ImageBrowserViewModel(m_exceptionPolicy);
            auto t = task<IVectorView<FileInformation^>^>(GetPhotosAsync());
            task_status status;
            auto files = TestHelper::RunSynced<IVectorView<FileInformation^>^>(t, status);
            auto photo = ref new Photo(files->GetAt(0), m_photoGroup, m_exceptionPolicy);

            TestHelper::RunUISynced([model, photo] {
                model->SelectedItem = photo;
            });

            Assert::IsTrue(model->IsAppBarEnabled);
        }

        TEST_METHOD(ImageBorwserViewModelShouldDisableAppBarWhenSettingTheSelectedItemToANullPtr)
        {
            auto model = ref new ImageBrowserViewModel(m_exceptionPolicy);

            TestHelper::RunUISynced([model] {
                model->SelectedItem = nullptr;
            });

            Assert::IsFalse(model->IsAppBarEnabled);
        }
      
        TEST_METHOD(ImageBrowserViewModelShouldFirePropertyChangeOfSelectedItemWhenSettingSelectedItem)
        {
            auto model = ref new ImageBrowserViewModel(m_exceptionPolicy);
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
            auto photo = ref new Photo(files->GetAt(0), m_photoGroup, m_exceptionPolicy);

            TestHelper::RunUISynced([model, photo] {
                model->SelectedItem = photo;
            });

            Assert::IsTrue(propertyChangedFired);
        }

        TEST_METHOD(ImageBrowserViewModelShouldFirePropertyChangeForIsAppBarEnabledWhenSettingSelectedItem)
        {
            auto model = ref new ImageBrowserViewModel(m_exceptionPolicy);
            bool propertyChangedFired = false;
            model->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e) 
            {
                if (e->PropertyName == "IsAppBarEnabled")
                {
                    propertyChangedFired = true;
                }
            });
            auto t = task<IVectorView<FileInformation^>^>(GetPhotosAsync());
            task_status status;
            auto files = TestHelper::RunSynced<IVectorView<FileInformation^>^>(t, status);
            auto photo = ref new Photo(files->GetAt(0), m_photoGroup, m_exceptionPolicy);

            TestHelper::RunUISynced([model, photo] {
                model->SelectedItem = photo;
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
        StubExceptionPolicy^ m_exceptionPolicy;
        StubPhotoGroup^ m_photoGroup;
	};
}

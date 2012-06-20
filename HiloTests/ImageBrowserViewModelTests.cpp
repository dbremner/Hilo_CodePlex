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
#include "..\Hilo\MonthBlock.h"
#include "..\Hilo\Photo.h"
#include "..\Hilo\YearGroup.h"
#include "StubExceptionPolicy.h"
#include "StubPhotoGroup.h"
#include "StubRepository.h"
#include "StubPhoto.h"

using namespace concurrency;
using namespace Hilo;
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
            m_exceptionPolicy = ref new StubExceptionPolicy();
            m_photoGroup = ref new StubPhotoGroup("");
            m_repository = ref new StubRepository();
        }

        TEST_METHOD(ImageBrowserViewModelShouldSetupGroupCommandWhenConstructed)
        {
            ImageBrowserViewModel model(m_repository, m_exceptionPolicy);
            Assert::IsNotNull(model.GroupCommand);
        }

        TEST_METHOD(ImageBrowserViewModelShouldSetupRotateCommandWhenConstructed)
        {
            ImageBrowserViewModel model(m_repository, m_exceptionPolicy);
            Assert::IsNotNull(model.RotateImageCommand);
        }

        TEST_METHOD(ImageBrowserViewModelShouldSetupCropCommandWhenConstructed)
        {
            ImageBrowserViewModel model(m_repository, m_exceptionPolicy);
            Assert::IsNotNull(model.CropImageCommand);
        }

        TEST_METHOD(ImageBrowserViewModelShouldDefaultToSelectedItemBeingNull)
        {
            ImageBrowserViewModel model(m_repository, m_exceptionPolicy);
            Assert::IsNull(model.SelectedItem);
        }
       
        /*TEST_METHOD(ImageBrowserViewModelShouldDisablePictureCommandsWhenSelectedItemIsNullptr)
        {
            auto model = ref new ImageBrowserViewModel(m_repository, m_exceptionPolicy);

            TestHelper::RunUISynced([model] {
                model->SelectedItem = ref new StubPhoto();
            });

            bool canExecuteCropCommand = true;
            model->CropImageCommand->CanExecuteChanged += ref new EventHandler<Object^>([model, &canExecuteCropCommand](Object^ sender, Object^ e)
            {
                canExecuteCropCommand = model->CropImageCommand->CanExecute(e);
            });

            bool canExecuteRotateCommand = true;
             model->RotateImageCommand->CanExecuteChanged += ref new EventHandler<Object^>([model, &canExecuteRotateCommand](Object^ sender, Object^ e)
        {
            auto model = ref new ImageBrowserViewModel(m_repository, m_exceptionPolicy);

            TestHelper::RunUISynced([model] {
                model->SelectedItem = nullptr;
            });

            Assert::IsFalse(canExecuteCropCommand);
            Assert::IsFalse(canExecuteRotateCommand);
        }
      
        TEST_METHOD(ImageBrowserViewModelShouldEnablePictureCommandsWhenSelectedItemIsNotNullptr)
        {
            auto model = ref new ImageBrowserViewModel(m_repository, m_exceptionPolicy);
            bool propertyChangedFired = false;
            model->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender,  PropertyChangedEventArgs^ e) 
            {
                canExecuteCropCommand = model->CropImageCommand->CanExecute(e);
            });

            bool canExecuteRotateCommand = false;
            model->RotateImageCommand->CanExecuteChanged += ref new EventHandler<Object^>([model, &canExecuteRotateCommand](Object^ sender, Object^ e)
                {
                canExecuteRotateCommand = model->RotateImageCommand->CanExecute(e);
            });

            TestHelper::RunUISynced([model] {
                model->SelectedItem = ref new StubPhoto();
            });

            Assert::IsTrue(canExecuteCropCommand);
            Assert::IsTrue(canExecuteRotateCommand);
        }*/

        TEST_METHOD(ImageBrowserViewModelShouldFirePropertyChangeOfSelectedItemWhenSettingSelectedItem)
        {
            auto model = ref new ImageBrowserViewModel(m_repository, m_exceptionPolicy);
            bool propertyChangedFired = false;
            model->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender,  PropertyChangedEventArgs^ e) 
            {
                if (e->PropertyName == "SelectedItem")
                {
                    propertyChangedFired = true;
                }
            });

            TestHelper::RunUISynced([model] {
                model->SelectedItem = ref new StubPhoto();
            });

            Assert::IsTrue(propertyChangedFired);
        }

        TEST_METHOD(ImageBrowserViewModelShouldCallRepositoryWhenQueryForMonthGroups)
        {
             auto model = ref new ImageBrowserViewModel(m_repository, m_exceptionPolicy);
            task_status status;
            
            TestHelper::RunSynced(model->QueryMonthGroupsAsync(), status);

            Assert::IsTrue(m_repository->GetMonthGroupedPhotosWithCacheAsyncCalled);
        }

        TEST_METHOD(ImageBrowserViewModelShouldCallRepositoryWhenQueryForYearGroups)
        {
             auto model = ref new ImageBrowserViewModel(m_repository, m_exceptionPolicy);
            task_status status;

            TestHelper::RunSynced(model->QueryYearGroupsAsync(), status);

            Assert::IsTrue(m_repository->GetYearGroupedMonthsAsyncCalled);
        }

    private:
        StubExceptionPolicy^ m_exceptionPolicy;
        StubPhotoGroup^ m_photoGroup;
        StubRepository^ m_repository;
	};
}

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
#include "StubExceptionPolicy.h"
#include "StubPhotoGroup.h"
#include "StubVirtualMonthFoldersQuery.h"
#include "StubVirtualYearFoldersQuery.h"
#include "StubPhoto.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Data;

namespace HiloTests
{
    TEST_CLASS(ImageBrowserViewModelTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_photoGroup = ref new StubPhotoGroup("");
            m_exceptionPolicy = std::make_shared<StubExceptionPolicy>();
            m_monthQuery = std::make_shared<StubVirtualMonthFoldersQuery>();
            m_yearQuery = std::make_shared<StubVirtualYearFoldersQuery>();
        }

        TEST_METHOD(ImageBrowserViewModelShouldSetupGroupCommandWhenConstructed)
        {
            ImageBrowserViewModel model(m_monthQuery, m_yearQuery, m_exceptionPolicy);
            Assert::IsNotNull(model.GroupCommand);
        }

        TEST_METHOD(ImageBrowserViewModelShouldSetupRotateCommandWhenConstructed)
        {
            ImageBrowserViewModel model(m_monthQuery, m_yearQuery, m_exceptionPolicy);
            Assert::IsNotNull(model.RotateImageCommand);
        }

        TEST_METHOD(ImageBrowserViewModelShouldSetupCropCommandWhenConstructed)
        {
            ImageBrowserViewModel model(m_monthQuery, m_yearQuery, m_exceptionPolicy);
            Assert::IsNotNull(model.CropImageCommand);
        }

        TEST_METHOD(ImageBrowserViewModelShouldDefaultToSelectedItemBeingNull)
        {
            ImageBrowserViewModel model(m_monthQuery, m_yearQuery, m_exceptionPolicy);
            Assert::IsNull(model.SelectedItem);
        }

        TEST_METHOD(ImageBrowserViewModelShouldAddItselfAsObserverOfQuery)
        {
            auto model = ref new ImageBrowserViewModel(m_monthQuery, m_yearQuery, m_exceptionPolicy);
            Assert::IsTrue(m_monthQuery->GetObserved());
        }

        TEST_METHOD(ImageBrowserViewModelShouldEnablePictureCommandsWhenSelectedItemIsNotNullptr)
        {
            auto model = ref new ImageBrowserViewModel(m_monthQuery, m_yearQuery, m_exceptionPolicy);
            bool canExecuteCropCommand = false;
            model->CropImageCommand->CanExecuteChanged += ref new EventHandler<Object^>([model, &canExecuteCropCommand](Object^ sender, Object^ e)
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
        }

        TEST_METHOD(ImageBrowserViewModelShouldDisablePictureCommandsWhenSelectedItemIsNullptr)
        {
            auto model = ref new ImageBrowserViewModel(m_monthQuery, m_yearQuery, m_exceptionPolicy);
            bool canExecuteCropCommand = true;
            model->CropImageCommand->CanExecuteChanged += ref new EventHandler<Object^>([model, &canExecuteCropCommand](Object^ sender, Object^ e)
            {
                canExecuteCropCommand = model->CropImageCommand->CanExecute(e);
            });
            bool canExecuteRotateCommand = true;
            model->RotateImageCommand->CanExecuteChanged += ref new EventHandler<Object^>([model, &canExecuteRotateCommand](Object^ sender, Object^ e)
            {
                canExecuteRotateCommand = model->RotateImageCommand->CanExecute(e);
            });

            TestHelper::RunUISynced([model] {
                model->SelectedItem = ref new StubPhoto();
                model->SelectedItem = nullptr;
            });

            Assert::IsFalse(canExecuteCropCommand);
            Assert::IsFalse(canExecuteRotateCommand);
        }

        TEST_METHOD(ImageBrowserViewModelShouldFirePropertyChangeOfSelectedItemWhenSettingSelectedItem)
        {
            auto model = ref new ImageBrowserViewModel(m_monthQuery, m_yearQuery, m_exceptionPolicy);
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
            auto model = ref new ImageBrowserViewModel(m_monthQuery, m_yearQuery, m_exceptionPolicy);
            task_status status;

            TestHelper::RunSynced(model->QueryMonthGroupsAsync(), status);
            Assert::IsTrue(m_monthQuery->GetHasBeenCalled());
        }

        TEST_METHOD(ImageBrowserViewModelShouldCallRepositoryWhenQueryForYearGroups)
        {
            auto model = ref new ImageBrowserViewModel(m_monthQuery, m_yearQuery, m_exceptionPolicy);
            task_status status;

            TestHelper::RunSynced(model->QueryYearGroupsAsync(), status);
            Assert::IsTrue(m_yearQuery->GetHasBeenCalled());
        }

    private:
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
        StubPhotoGroup^ m_photoGroup;
        std::shared_ptr<StubVirtualMonthFoldersQuery> m_monthQuery;
        std::shared_ptr<StubVirtualYearFoldersQuery> m_yearQuery;
    };
}

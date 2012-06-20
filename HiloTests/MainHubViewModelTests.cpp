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
#include "StubRepository.h"
#include "StubPhoto.h"

using namespace concurrency;
using namespace Hilo;
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
            m_exceptionPolicy = ref new StubExceptionPolicy();
            m_photoGroup = ref new StubPhotoGroup("");
            m_repository = ref new StubRepository();
        }

        TEST_METHOD_CLEANUP(Cleanup)
        {
        }

        TEST_METHOD(MainHubViewModelShouldGetPhotoGroupsForHub)
        {
            auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);
            auto model = ref new MainHubViewModel(vector, m_exceptionPolicy);
            auto photoGroupTasks = create_task([model]()-> Object^ {
                return model->PhotoGroups;
            });
            task_status status;

            Object^ photoGroups = TestHelper::RunSynced<Object^>(photoGroupTasks, status);

            Assert::AreEqual(completed, status);
            Assert::IsNotNull(photoGroups);
        }

        TEST_METHOD(MainHubViewModelShouldGetPhotoGroupsAsIObservableVectorOfPhotoGroups)
        {
            auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);
            TypeName pageType;
            auto model = ref new MainHubViewModel(vector, m_exceptionPolicy);
            auto photoGroupTasks = create_task([model]()-> Object^ {
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
            auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);

            MainHubViewModel model(vector, m_exceptionPolicy);

            Assert::IsNotNull(model.NavigateToPicturesCommand);
        }

        TEST_METHOD(MainHubViewModelShouldSetupRotateCommandWhenConstructed)
        {
            auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);

            MainHubViewModel model(vector, m_exceptionPolicy);

            Assert::IsNotNull(model.RotateImageCommand);
        }

        TEST_METHOD(MainHubViewModelShouldSetupCropCommandWhenConstructed)
        {
            auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);

            MainHubViewModel model(vector, m_exceptionPolicy);

            Assert::IsNotNull(model.CropImageCommand);
        }

        TEST_METHOD(MainHubViewModelShouldDefaultToSelectedItemBeingNull)
        {
            auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);

            MainHubViewModel model(vector, m_exceptionPolicy);

            Assert::IsNull(model.SelectedItem);
        }

        TEST_METHOD(MainHubViewModelShouldDisablePictureCommandsWhenSelectedItemIsNullptr)
        {
            auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);
            auto model = ref new MainHubViewModel(vector, m_exceptionPolicy);

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
                canExecuteRotateCommand = model->RotateImageCommand->CanExecute(e);
            });

            TestHelper::RunUISynced([model] {
                model->SelectedItem = nullptr;
            });

            Assert::IsFalse(canExecuteCropCommand);
            Assert::IsFalse(canExecuteRotateCommand);
        }

        TEST_METHOD(MainHubViewModelShouldEnablePictureCommandsWhenSelectedItemIsNotNullptr)
        {
            auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);
            auto model = ref new MainHubViewModel(vector, m_exceptionPolicy);

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



        TEST_METHOD(MainHubViewModelShouldFirePropertyChangeForSelectedItemWhenSettingSelectedItem)
        {
            auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
            auto vector = ref new Vector<HubPhotoGroup^>();
            vector->Append(photoGroup);            
            auto model = ref new MainHubViewModel(vector, m_exceptionPolicy);
            bool propertyChangedFired = false;
            model->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender,  PropertyChangedEventArgs^ e) 
            {
                if (e->PropertyName == "SelectedItem")
                {
                    propertyChangedFired = true;
                }
            });
            auto photo = ref new StubPhoto();

            TestHelper::RunUISynced([model, photo] {
                model->SelectedItem = photo;
            });

            Assert::IsTrue(propertyChangedFired);
        }

    private:
        StubExceptionPolicy^ m_exceptionPolicy;
        StubPhotoGroup^ m_photoGroup;
        StubRepository^ m_repository;
    };
}
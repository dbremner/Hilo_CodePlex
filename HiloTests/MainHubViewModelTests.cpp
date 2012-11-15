// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "CppUnitTest.h"
#include "UnitTestingAssertSpecializations.h"
#include "..\Hilo\HubPhotoGroup.h"
#include "..\Hilo\MainHubViewModel.h"
#include "StubExceptionPolicy.h"
#include "StubPhotoGroup.h"
#include "StubPhoto.h"
#include "StubRepository.h"

using namespace concurrency;
using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;

namespace HiloTests
{
    TEST_CLASS(MainHubViewModelTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_photoGroup = ref new StubPhotoGroup("");
            m_exceptionPolicy = std::make_shared<StubExceptionPolicy>();
            m_repository = std::make_shared<StubRepository>(m_exceptionPolicy);
        }

        TEST_METHOD(MainHubViewModelShouldGetPhotoGroupsForHub)
        {
            auto vm = std::make_shared<MainHubViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
                auto vector = ref new Vector<HubPhotoGroup^>();
                vector->Append(photoGroup);
                (*vm) = ref new MainHubViewModel(vector, m_exceptionPolicy);
            });

            auto photoGroupTasks = create_task([vm]()-> Object^ 
            {
                return (*vm)->PhotoGroups;
            });
            task_status status;

            Object^ photoGroups = TestHelper::RunSynced<Object^>(photoGroupTasks, status);

            Assert::AreEqual(completed, status);
            Assert::IsNotNull(photoGroups);
        }

        TEST_METHOD(MainHubViewModelShouldGetPhotoGroupsAsIObservableVectorOfPhotoGroups)
        {
            auto vm = std::make_shared<MainHubViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
                auto vector = ref new Vector<HubPhotoGroup^>();
                vector->Append(photoGroup);
                (*vm) = ref new MainHubViewModel(vector, m_exceptionPolicy);
            });

            auto photoGroupTasks = create_task([vm]()-> Object^ 
            {
                return (*vm)->PhotoGroups;
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
            auto vm = std::make_shared<MainHubViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
                auto vector = ref new Vector<HubPhotoGroup^>();
                vector->Append(photoGroup);
                (*vm) = ref new MainHubViewModel(vector, m_exceptionPolicy);
            });

            Assert::IsNotNull((*vm)->NavigateToPicturesCommand);
        }

        TEST_METHOD(MainHubViewModelShouldSetupRotateCommandWhenConstructed)
        {
            auto vm = std::make_shared<MainHubViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
                auto vector = ref new Vector<HubPhotoGroup^>();
                vector->Append(photoGroup);
                (*vm) = ref new MainHubViewModel(vector, m_exceptionPolicy);
            });

            Assert::IsNotNull((*vm)->RotateImageCommand);
        }

        TEST_METHOD(MainHubViewModelShouldSetupCropCommandWhenConstructed)
        {
            auto vm = std::make_shared<MainHubViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
                auto vector = ref new Vector<HubPhotoGroup^>();
                vector->Append(photoGroup);
                (*vm) = ref new MainHubViewModel(vector, m_exceptionPolicy);
            });

            Assert::IsNotNull((*vm)->CropImageCommand);
        }

        TEST_METHOD(MainHubViewModelShouldSetupCartoonizeCommandWhenConstructed)
        {
            auto vm = std::make_shared<MainHubViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]()
            {
                auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
                auto vector = ref new Vector<HubPhotoGroup^>();
                vector->Append(photoGroup);
                (*vm) = ref new MainHubViewModel(vector, m_exceptionPolicy);
            });

            Assert::IsNotNull((*vm)->CartoonizeImageCommand);
        }

        TEST_METHOD(MainHubViewModelShouldDefaultToSelectedItemBeingNull)
        {
            auto vm = std::make_shared<MainHubViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
                auto vector = ref new Vector<HubPhotoGroup^>();
                vector->Append(photoGroup);
                (*vm) = ref new MainHubViewModel(vector, m_exceptionPolicy);
            });

            Assert::IsNull((*vm)->SelectedItem);
        }
      
        TEST_METHOD(MainHubViewModelShouldEnableAppBarWhenSettingTheSelectedItemToAPhoto)
        {
            auto vm = std::make_shared<MainHubViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
                auto vector = ref new Vector<HubPhotoGroup^>();
                vector->Append(photoGroup);
                (*vm) = ref new MainHubViewModel(vector, m_exceptionPolicy);
                (*vm)->SelectedItem = ref new StubPhoto();
            });    

            Assert::IsTrue((*vm)->IsAppBarEnabled);
        }

        TEST_METHOD(MainHubViewModelShouldDisableAppBarWhenSettingTheSelectedItemToANullPtr)
        {
            auto vm = std::make_shared<MainHubViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
                auto vector = ref new Vector<HubPhotoGroup^>();
                vector->Append(photoGroup);
                (*vm) = ref new MainHubViewModel(vector, m_exceptionPolicy);
                (*vm)->SelectedItem = nullptr;
            });    

            Assert::IsFalse((*vm)->IsAppBarEnabled);
        }
        
        TEST_METHOD(MainHubViewModelShouldFirePropertyChangeForSelectedItemWhenSettingSelectedItem)
        {
            bool propertyChangedFired = false; 
            auto vm = std::make_shared<MainHubViewModel^>(nullptr);

            TestHelper::RunUISynced([this, vm, &propertyChangedFired]() 
            {
                auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
                auto vector = ref new Vector<HubPhotoGroup^>();
                vector->Append(photoGroup);
                (*vm) = ref new MainHubViewModel(vector, m_exceptionPolicy);

                (*vm)->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender,  PropertyChangedEventArgs^ e) 
                {
                    if (e->PropertyName == "SelectedItem")
                    {
                        propertyChangedFired = true;
                    }
                });

                (*vm)->SelectedItem = ref new StubPhoto();
            });

            Assert::IsTrue(propertyChangedFired);
        }

        TEST_METHOD(MainHubViewModelShouldFirePropertyChangeForIsAppBarEnabledWhenSettingSelectedItem)
        {
            bool propertyChangedFired = false; 
            auto vm = std::make_shared<MainHubViewModel^>(nullptr);

            TestHelper::RunUISynced([this, vm, &propertyChangedFired]() 
            {
                auto photoGroup = ref new HubPhotoGroup("Title", "Empty Title", m_repository, m_exceptionPolicy);
                auto vector = ref new Vector<HubPhotoGroup^>();
                vector->Append(photoGroup);
                (*vm) = ref new MainHubViewModel(vector, m_exceptionPolicy);

                (*vm)->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender,  PropertyChangedEventArgs^ e) 
                {
                    if (e->PropertyName == "IsAppBarEnabled")
                    {
                        propertyChangedFired = true;
                    }
                });

                (*vm)->SelectedItem = ref new StubPhoto();
            });           
        }

    private:
        StubPhotoGroup^ m_photoGroup;
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
        std::shared_ptr<StubRepository> m_repository;
    };
}
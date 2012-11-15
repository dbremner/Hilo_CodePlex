// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "CppUnitTest.h"
#include "..\Hilo\ImageBrowserViewModel.h"
#include "StubExceptionPolicy.h"
#include "StubPhotoGroup.h"
#include "StubPhoto.h"
#include "StubRepository.h"

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
            m_repository = std::make_shared<StubRepository>(m_exceptionPolicy);
        }

        TEST_METHOD(ImageBrowserViewModelShouldSetupGroupCommandWhenConstructed)
        {
            auto vm = std::make_shared<ImageBrowserViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new ImageBrowserViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNotNull((*vm)->GroupCommand);
        }

        TEST_METHOD(ImageBrowserViewModelShouldSetupRotateCommandWhenConstructed)
        {
            auto vm = std::make_shared<ImageBrowserViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new ImageBrowserViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNotNull((*vm)->RotateImageCommand);
        }

        TEST_METHOD(ImageBrowserViewModelShouldSetupCropCommandWhenConstructed)
        {
            auto vm = std::make_shared<ImageBrowserViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new ImageBrowserViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNotNull((*vm)->CropImageCommand);
        }

        TEST_METHOD(ImageBrowserViewModelShouldSetupCartoonizeCommandWhenConstructed)
        {
            auto vm = std::make_shared<ImageBrowserViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]()
            {
                (*vm) = ref new ImageBrowserViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNotNull((*vm)->CartoonizeImageCommand);
        }

        TEST_METHOD(ImageBrowserViewModelShouldDefaultToSelectedItemBeingNull)
        {
            auto vm = std::make_shared<ImageBrowserViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new ImageBrowserViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNull((*vm)->SelectedItem);
        }

        TEST_METHOD(ImageBrowserViewModelShouldAddItselfAsObserverOfQuery)
        {
            auto vm = std::make_shared<ImageBrowserViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new ImageBrowserViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsTrue(m_repository->GetAddObserverHasBeenCalled());
        }

        TEST_METHOD(ImageBrowserViewModelShouldEnableAppBarWhenSettingTheSelectedItemToAPhoto)
        {
            auto vm = std::make_shared<ImageBrowserViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new ImageBrowserViewModel(m_repository, m_exceptionPolicy);
                (*vm)->SelectedItem = ref new StubPhoto();
            });
            Assert::IsTrue((*vm)->IsAppBarEnabled);
        }

        TEST_METHOD(ImageBrowserViewModelShouldDisableAppBarWhenSettingTheSelectedItemToANullPtr)
        {
            auto vm = std::make_shared<ImageBrowserViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new ImageBrowserViewModel(m_repository, m_exceptionPolicy);
                (*vm)->SelectedItem = nullptr;
            });
            Assert::IsFalse((*vm)->IsAppBarEnabled);
        }

        TEST_METHOD(ImageBrowserViewModelShouldFirePropertyChangeOfSelectedItemWhenSettingSelectedItem)
        {
            auto vm = std::make_shared<ImageBrowserViewModel^>(nullptr);
            bool propertyChangedFired = false;

            TestHelper::RunUISynced([this, vm, &propertyChangedFired]() 
            {
                (*vm) = ref new ImageBrowserViewModel(m_repository, m_exceptionPolicy);
                (*vm)->SelectedItem = nullptr;
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

        TEST_METHOD(ImageBrowserViewModelShouldFirePropertyChangeForIsAppBarEnabledWhenSettingSelectedItem)
        {
            auto vm = std::make_shared<ImageBrowserViewModel^>(nullptr);
            bool propertyChangedFired = false;

            TestHelper::RunUISynced([this, vm, &propertyChangedFired]() 
            {
                (*vm) = ref new ImageBrowserViewModel(m_repository, m_exceptionPolicy);
                (*vm)->SelectedItem = nullptr;
                (*vm)->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender,  PropertyChangedEventArgs^ e) 
                {
                    if (e->PropertyName == "IsAppBarEnabled")
                    {
                        propertyChangedFired = true;
                    }
                });
                (*vm)->SelectedItem = ref new StubPhoto();
            });

            Assert::IsTrue(propertyChangedFired);
        }

    private:
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
        StubPhotoGroup^ m_photoGroup;
        std::shared_ptr<StubRepository> m_repository;
    };
}

// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "CppUnitTest.h"
#include "UnitTestingAssertSpecializations.h"
#include <tuple>
#include "..\Hilo\RotateImageViewModel.h"
#include "..\Hilo\ImageNavigationData.h"
#include "StubExceptionPolicy.h"
#include "StubPhotoGroup.h"
#include "StubPhoto.h"
#include "StubRepository.h"

using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Data;

namespace HiloTests
{
    TEST_CLASS(RotateImageViewModelTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_exceptionPolicy = std::make_shared<StubExceptionPolicy>();
            m_repository = std::make_shared<StubRepository>(m_exceptionPolicy);
        }

        TEST_METHOD(RotateImageViewModelShouldSetupRotateCommandWhenConstructed)
        {
            auto vm = std::make_shared<RotateImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new RotateImageViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNotNull((*vm)->RotateCommand);
        }

        TEST_METHOD(RotateImageViewModelShouldSetupResumeRotateCommandWhenConstructed)
        {
            auto vm = std::make_shared<RotateImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new RotateImageViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNotNull((*vm)->ResumeRotateCommand);
        }

        TEST_METHOD(RotateImageViewModelShouldSetupSaveCommandWhenConstructed)
        {
            auto vm = std::make_shared<RotateImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new RotateImageViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNotNull((*vm)->SaveCommand);
        }

        TEST_METHOD(RotateImageViewModelShouldSetupCancelCommandWhenConstructed)
        {
            auto vm = std::make_shared<RotateImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new RotateImageViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNotNull((*vm)->CancelCommand);
        }

        TEST_METHOD(RotateImageViewModelShouldSetIsAppBarStickyWhenConstructed)
        {
            auto vm = std::make_shared<RotateImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new RotateImageViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsTrue((*vm)->IsAppBarSticky);
        }

        TEST_METHOD(RotateImageViewModelShouldSetImageMarginToZeroWhenSettingRotationAngleToZero)
        {
            auto vm = std::make_shared<RotateImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new RotateImageViewModel(m_repository, m_exceptionPolicy);
                (*vm)->RotationAngle = 0;
            });

            auto marginValue = (*vm)->ImageMargin;
            Assert::AreEqual(marginValue.Left, 0.0);
            Assert::AreEqual(marginValue.Top, 0.0);
            Assert::AreEqual(marginValue.Right, 0.0);
            Assert::AreEqual(marginValue.Bottom, 0.0);
        }

        TEST_METHOD(RotateImageViewModelShouldSetImageMarginTopAndBottomToOneHundredAndTenWhenSettingRotationAngleToNinety)
        {
            auto vm = std::make_shared<RotateImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new RotateImageViewModel(m_repository, m_exceptionPolicy);
                (*vm)->RotationAngle = 90;
            });

            auto marginValue = (*vm)->ImageMargin;
            Assert::AreEqual(marginValue.Left, 0.0);
            Assert::AreEqual(marginValue.Top, 110.0);
            Assert::AreEqual(marginValue.Right, 0.0);
            Assert::AreEqual(marginValue.Bottom, 110.0);
        }

        TEST_METHOD(RotateImageViewModelShouldFirePropertyChangedForRotationAngleWhenSettingRotationAngle)
        {
            bool propertyChangedFired = false;
            auto vm = std::make_shared<RotateImageViewModel^>(nullptr);

            TestHelper::RunUISynced([this, vm, &propertyChangedFired]() 
            {
                (*vm) = ref new RotateImageViewModel(m_repository, m_exceptionPolicy);
                (*vm)->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e)
                {
                    if (e->PropertyName == "RotationAngle")
                    {
                        propertyChangedFired = true;
                    }
                });
                (*vm)->RotationAngle = 90;
            });

            Assert::IsTrue(propertyChangedFired);
        }

        TEST_METHOD(RotateImageViewModelShouldFirePropertyChangedForImageMarginWhenSettingRotationAngle)
        {
            bool propertyChangedFired = false;
            auto vm = std::make_shared<RotateImageViewModel^>(nullptr);

            TestHelper::RunUISynced([this, vm, &propertyChangedFired]() 
            {
                (*vm) = ref new RotateImageViewModel(m_repository, m_exceptionPolicy);
                (*vm)->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e)
                {
                    if (e->PropertyName == "ImageMargin")
                    {
                        propertyChangedFired = true;
                    }
                });
                (*vm)->RotationAngle = 90;
            });

            Assert::IsTrue(propertyChangedFired);
        }

        TEST_METHOD(RotateImageViewModelInitialRotationAngleIsZero)
        {
            auto vm = std::make_shared<RotateImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new RotateImageViewModel(m_repository, m_exceptionPolicy);
                (*vm)->Initialize("");
            });

            Assert::AreEqual(0.0, (*vm)->RotationAngle);
        }

        TEST_METHOD(RotateImageViewModelSnapsToNearest90Angle)
        {
            std::tuple<float64, float64> testAngles[] = 
            {
                std::make_tuple(0, 0),
                std::make_tuple(44.9, 0),
                std::make_tuple(45.1, 90),
                std::make_tuple(92.4, 90),
                std::make_tuple(135.1, 180),
                std::make_tuple(181.001, 180),
                std::make_tuple(225.001, 270),
                std::make_tuple(310.21, 270),
                std::make_tuple(315.87, 0),
                std::make_tuple(359.99, 0),
                std::make_tuple(361.1, 0)
            };
            for (auto item : testAngles)
            {
                TestRotation(std::get<0>(item), std::get<1>(item));
            }

        }

        TEST_METHOD(RotateImageViewModelShouldSaveAndLoadRotationAngle)
        {
            auto vm = std::make_shared<RotateImageViewModel^>(nullptr);
            auto newVm = std::make_shared<RotateImageViewModel^>(nullptr);

            TestHelper::RunUISynced([this, vm, newVm]() 
            {
                (*vm) = ref new RotateImageViewModel(m_repository, m_exceptionPolicy);
                (*vm)->RotationAngle = 90;
                auto state = ref new Platform::Collections::Map<String^, Object^>();
                (*vm)->SaveState(state);

                (*newVm) = ref new RotateImageViewModel(m_repository, m_exceptionPolicy);
                (*newVm)->LoadState(state);
            });

            Assert::AreEqual((*vm)->RotationAngle,  (*newVm)->RotationAngle);
        }

        TEST_METHOD(RotateImageViewModelShouldSaveAndLoadMargin)
        {
            auto vm = std::make_shared<RotateImageViewModel^>(nullptr);
            auto newVm = std::make_shared<RotateImageViewModel^>(nullptr);

            TestHelper::RunUISynced([this, vm, newVm]() 
            {
                (*vm) = ref new RotateImageViewModel(m_repository, m_exceptionPolicy);
                (*vm)->RotationAngle = 270; // Should set margin to something non-zero
                auto state = ref new Platform::Collections::Map<String^, Object^>();
                (*vm)->SaveState(state);

                (*newVm) = ref new RotateImageViewModel(m_repository, m_exceptionPolicy);
                (*newVm)->LoadState(state);
            });

            Assert::AreEqual((*vm)->ImageMargin, (*newVm)->ImageMargin);
        }

        TEST_METHOD(RotateImageViewModelRotateCommandIncrementsBy90)
        {
            auto vm = std::make_shared<RotateImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new RotateImageViewModel(m_repository, m_exceptionPolicy);
                (*vm)->Initialize("");
                (*vm)->RotateCommand->Execute(nullptr);
            });

            Assert::AreEqual(90.0, (*vm)->RotationAngle);
        }

    private:
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
        std::shared_ptr<StubRepository> m_repository;

        void TestRotation(float64 angle, float64 expected)
        {
            auto vm = std::make_shared<RotateImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm, angle]()
            {
                (*vm) = ref new RotateImageViewModel(m_repository, m_exceptionPolicy);
                (*vm)->Initialize("");
                (*vm)->RotationAngle = angle;
                (*vm)->EndRotation();
            });

            auto message = ref new Platform::String(L"Unexpected result rotating to angle ");
            message += angle.ToString();
            Assert::AreEqual(expected, (*vm)->RotationAngle, message->Data());
        }
    };
}

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
#include <tuple>
#include "..\Hilo\RotateImageViewModel.h"
#include "..\Hilo\ImageNavigationData.h"
#include "StubExceptionPolicy.h"
#include "StubPhotoGroup.h"
#include "StubPhoto.h"
#include "StubSinglePhotoQuery.h"

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
            m_query = std::make_shared<StubSinglePhotoQuery>();
            m_exceptionPolicy = std::make_shared<StubExceptionPolicy>();
        }

        TEST_METHOD(RotateImageViewModelShouldSetupRotateCommandWhenConstructed)
        {
            RotateImageViewModel model(m_query, m_exceptionPolicy);
            Assert::IsNotNull(model.RotateCommand);
        }

        TEST_METHOD(RotateImageViewModelShouldSetupResumeRotateCommandWhenConstructed)
        {
            RotateImageViewModel model(m_query, m_exceptionPolicy);
            Assert::IsNotNull(model.ResumeRotateCommand);
        }

        TEST_METHOD(RotateImageViewModelShouldSetupSaveCommandWhenConstructed)
        {
            RotateImageViewModel model(m_query, m_exceptionPolicy);
            Assert::IsNotNull(model.SaveCommand);
        }

        TEST_METHOD(RotateImageViewModelShouldSetupCancelCommandWhenConstructed)
        {
            RotateImageViewModel model(m_query, m_exceptionPolicy);
            Assert::IsNotNull(model.CancelCommand);
        }

        TEST_METHOD(RotateImageViewModelShouldSetIsAppBarStickyWhenConstructed)
        {
            RotateImageViewModel model(m_query, m_exceptionPolicy);
            Assert::IsTrue(model.IsAppBarSticky);
        }

        TEST_METHOD(RotateImageViewModelShouldSetImageMarginToZeroWhenSettingRotationAngleToZero)
        {
            auto model = ref new RotateImageViewModel(m_query, m_exceptionPolicy);

            TestHelper::RunUISynced([model] {
                model->RotationAngle = 0;
            });

            auto marginValue = model->ImageMargin;
            Assert::AreEqual(marginValue.Left, 0.0);
            Assert::AreEqual(marginValue.Top, 0.0);
            Assert::AreEqual(marginValue.Right, 0.0);
            Assert::AreEqual(marginValue.Bottom, 0.0);
        }

        TEST_METHOD(RotateImageViewModelShouldSetImageMarginTopAndBottomToOneHundredAndTenWhenSettingRotationAngleToNinety)
        {
            auto model = ref new RotateImageViewModel(m_query, m_exceptionPolicy);

            TestHelper::RunUISynced([model] {
                model->RotationAngle = 90;
            });

            auto marginValue = model->ImageMargin;
            Assert::AreEqual(marginValue.Left, 0.0);
            Assert::AreEqual(marginValue.Top, 110.0);
            Assert::AreEqual(marginValue.Right, 0.0);
            Assert::AreEqual(marginValue.Bottom, 110.0);
        }

        TEST_METHOD(RotateImageViewModelShouldFirePropertyChangedForRotationAngleWhenSettingRotationAngle)
        {
            auto model = ref new RotateImageViewModel(m_query, m_exceptionPolicy);
            bool propertyChangedFired = false;
            model->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e)
            {
                if (e->PropertyName == "RotationAngle")
                {
                    propertyChangedFired = true;
                }
            });

            TestHelper::RunUISynced([model] {
                model->RotationAngle = 90;
            });

            Assert::IsTrue(propertyChangedFired);
        }

        TEST_METHOD(RotateImageViewModelShouldFirePropertyChangedForImageMarginWhenSettingRotationAngle)
        {
            auto model = ref new RotateImageViewModel(m_query, m_exceptionPolicy);
            bool propertyChangedFired = false;
            model->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e)
            {
                if (e->PropertyName == "ImageMargin")
                {
                    propertyChangedFired = true;
                }
            });

            TestHelper::RunUISynced([model] {
                model->RotationAngle = 90;
            });

            Assert::IsTrue(propertyChangedFired);
        }

        TEST_METHOD(RotateImageViewModelInitialRotationAngleIsZero)
        {
            RotateImageViewModel^ vm = ref new RotateImageViewModel(m_query, m_exceptionPolicy);

            vm->Initialize("");

            Assert::AreEqual(0.0, vm->RotationAngle);
        }

        TEST_METHOD(RotateImageViewModelSnapsToNearest90Angle)
        {
            std::tuple<double,double> testAngles[] = {
                std::make_tuple(0,0),
                std::make_tuple(44.9,0),
                std::make_tuple(45.1,90),
                std::make_tuple(92.4,90),
                std::make_tuple(135.1,180),
                std::make_tuple(181.001,180),
                std::make_tuple(225.001,270),
                std::make_tuple(310.21,270),
                std::make_tuple(315.87,0),
                std::make_tuple(359.99,0),
                std::make_tuple(361.1,0)
            };
            for (auto item : testAngles) {
                TestRotation(std::get<0>(item), std::get<1>(item));
            }

        }

        TEST_METHOD(RotateImageViewModelShouldSaveAndLoadRotationAngle)
        {
            auto model = ref new RotateImageViewModel(m_query, m_exceptionPolicy);

            TestHelper::RunUISynced([model] {
                model->RotationAngle = 90;
            });

            auto state = ref new Platform::Collections::Map<String^, Object^>();
            model->SaveState(state);

            auto newModel = ref new RotateImageViewModel(m_query, m_exceptionPolicy);
            newModel->LoadState(state);

            Assert::AreEqual(model->RotationAngle, newModel->RotationAngle);
        }

        TEST_METHOD(RotateImageViewModelShouldSaveAndLoadMargin)
        {
            auto model = ref new RotateImageViewModel(m_query, m_exceptionPolicy);

            TestHelper::RunUISynced([model] {
                model->RotationAngle = 270; // Should set margin to something non-zero
            });

            auto state = ref new Platform::Collections::Map<String^, Object^>();
            model->SaveState(state);

            auto newModel = ref new RotateImageViewModel(m_query, m_exceptionPolicy);
            newModel->LoadState(state);

            Assert::AreEqual(model->ImageMargin, newModel->ImageMargin);
        }

        TEST_METHOD(RotateImageViewModelRotateCommandIncrementsBy90)
        {
            RotateImageViewModel^ vm = ref new RotateImageViewModel(m_query, m_exceptionPolicy);

            vm->Initialize("");

            TestHelper::RunUISynced([this, vm](){
                vm->RotateCommand->Execute(nullptr);
            });

            Assert::AreEqual(90.0, vm->RotationAngle);
        }

    private:
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
        std::shared_ptr<StubSinglePhotoQuery> m_query;

        void TestRotation(double angle, double expected)
        {
            RotateImageViewModel^ vm = ref new RotateImageViewModel(m_query, m_exceptionPolicy);

            vm->Initialize("");

            TestHelper::RunUISynced([this, vm, angle](){
                vm->RotationAngle = angle;
                vm->EndRotation();
            });

            auto message = ref new Platform::String(L"Unexpected result rotating to angle ");
            message += angle.ToString();
            Assert::AreEqual(expected, vm->RotationAngle, message->Data());
        }
    };
}

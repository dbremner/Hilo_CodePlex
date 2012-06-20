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
#include "..\Hilo\CropImageViewModel.h"
#include "StubExceptionPolicy.h"
#include "StubRepository.h"

using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;

namespace HiloTests
{
    TEST_CLASS(CropImageViewModelTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_repository = ref new StubRepository();
            m_exceptionPolicy = ref new StubExceptionPolicy();
        }

        TEST_METHOD(CropImageViewModelIsNotNull)
        {
            auto vm = ref new CropImageViewModel(m_repository, m_exceptionPolicy);
            Assert::IsNotNull(vm, "CropImageViewModel should not be null");
        }

        TEST_METHOD(CropImageViewModelSaveCommandIsNotNull)
        {
            auto vm = ref new CropImageViewModel(m_repository, m_exceptionPolicy);
            ICommand^ saveCommand = vm->SaveCommand;
            Assert::IsNotNull(saveCommand, "Save command should not be null");
        }

        TEST_METHOD(CropImageViewModelCancelCommandIsNotNull)
        {
            auto vm = ref new CropImageViewModel(m_repository, m_exceptionPolicy);
            ICommand^ cancelCommand = vm->CancelCommand;
            Assert::IsNotNull(cancelCommand, "Cancel command should not be null");
        }

        TEST_METHOD(CropImageViewModelCalculateCropOverlayPositionSetsIsCropOverlayVisibleToTrue)
        {
            auto vm = ref new CropImageViewModel(m_repository, m_exceptionPolicy);
            bool propertyChangedFired = false;
            vm->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e) {
                propertyChangedFired = true;
            });
            GeneralTransform^ gt;

            TestHelper::RunUISynced([vm, gt]() {
                vm->CalculateInitialCropOverlayPosition(gt, 800.0, 600.0);
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::AreEqual(vm->IsCropOverlayVisible, true);
        }

        TEST_METHOD(CropImageViewModelCalculateCropOverlayPositionSetsCorrectCropOverlayWidthAndHeight)
        {
            auto vm = ref new CropImageViewModel(m_repository, m_exceptionPolicy);
            bool propertyChangedFired = false;
            vm->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e) {
                propertyChangedFired = true;
            });
            GeneralTransform^ gt;

            TestHelper::RunUISynced([vm, gt]() {
                vm->CalculateInitialCropOverlayPosition(gt, 800.0, 600.0);
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::AreEqual(vm->CropOverlayWidth, 800.0);
            Assert::AreEqual(vm->CropOverlayHeight, 600.0);
        }

        TEST_METHOD(CropImageViewModelCalculateCropOverlayPositionSetsCorrectCropOverlayLeftAndTop)
        {
            auto vm = ref new CropImageViewModel(m_repository, m_exceptionPolicy);
            bool propertyChangedFired = false;
            vm->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e) {
                propertyChangedFired = true;
            });
            GeneralTransform^ gt;

            TestHelper::RunUISynced([vm, gt]() {
                vm->CalculateInitialCropOverlayPosition(gt, 800.0, 600.0);
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::AreEqual(vm->CropOverlayLeft, 0.0);
            Assert::AreEqual(vm->CropOverlayTop, 0.0);
        }

        TEST_METHOD(CropImageViewModelUpdateCropOverlayPositionUpdatesCropOverlayHeight)
        {
            auto vm = ref new CropImageViewModel(m_repository, m_exceptionPolicy);
            bool propertyChangedFired = false;
            vm->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e) {
                propertyChangedFired = true;
            });
            GeneralTransform^ gt;

            TestHelper::RunUISynced([vm, gt]() {
                Thumb^ thumb = ref new Thumb();
                thumb->VerticalAlignment = VerticalAlignment::Bottom;
                vm->CalculateInitialCropOverlayPosition(gt, 800.0, 600.0);
                vm->UpdateCropOverlayPostion(thumb, -2.5, 0.0, 100, 100);
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::AreEqual(vm->CropOverlayHeight, 597.5);
        }

        TEST_METHOD(CropImageViewModelUpdateCropOverlayPositionUpdatesCropOverlayHeightAndTop)
        {
            auto vm = ref new CropImageViewModel(m_repository, m_exceptionPolicy);
            bool propertyChangedFired = false;
            vm->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e) {
                propertyChangedFired = true;
            });
            GeneralTransform^ gt;

            TestHelper::RunUISynced([vm, gt]() {
                Thumb^ thumb = ref new Thumb();
                thumb->VerticalAlignment = VerticalAlignment::Top;
                vm->CalculateInitialCropOverlayPosition(gt, 800.0, 600.0);
                vm->UpdateCropOverlayPostion(thumb, 2.5, 0.0, 100, 100);
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::AreEqual(vm->CropOverlayTop, 2.5);
            Assert::AreEqual(vm->CropOverlayHeight, 597.5);
        }

        TEST_METHOD(CropImageViewModelUpdateCropOverlayPositionUpdatesCropOverlayWidth)
        {
            auto vm = ref new CropImageViewModel(m_repository, m_exceptionPolicy);
            bool propertyChangedFired = false;
            vm->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e) {
                propertyChangedFired = true;
            });
            GeneralTransform^ gt;

            TestHelper::RunUISynced([vm, gt]() {
                Thumb^ thumb = ref new Thumb();
                thumb->HorizontalAlignment = HorizontalAlignment::Right;
                vm->CalculateInitialCropOverlayPosition(gt, 800.0, 600.0);
                vm->UpdateCropOverlayPostion(thumb, 0.0, -2.5, 100, 100);
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::AreEqual(vm->CropOverlayWidth, 797.5);
        }

        TEST_METHOD(CropImageViewModelUpdateCropOverlayPositionUpdatesCropOverlayLeftAndWidth)
        {
            auto vm = ref new CropImageViewModel(m_repository, m_exceptionPolicy);
            bool propertyChangedFired = false;
            vm->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e) {
                propertyChangedFired = true;
            });
            GeneralTransform^ gt;

            TestHelper::RunUISynced([vm, gt]() {
                Thumb^ thumb = ref new Thumb();
                thumb->HorizontalAlignment = HorizontalAlignment::Left;
                vm->CalculateInitialCropOverlayPosition(gt, 800.0, 600.0);
                vm->UpdateCropOverlayPostion(thumb, 0.0, 2.5, 100, 100);
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::AreEqual(vm->CropOverlayLeft, 2.5);
            Assert::AreEqual(vm->CropOverlayWidth, 797.5);
        }

    private:
        StubRepository^ m_repository;
        StubExceptionPolicy^ m_exceptionPolicy;
    };
}
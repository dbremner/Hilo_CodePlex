// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
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
using namespace Windows::UI::Xaml::Media;

namespace HiloTests
{
    TEST_CLASS(CropImageViewModelTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_exceptionPolicy = std::make_shared<StubExceptionPolicy>();
            m_repository = std::make_shared<StubRepository>(m_exceptionPolicy);
        }

        TEST_METHOD(CropImageViewModelShouldSetupSaveCommandWhenConstructed)
        {
            auto vm = std::make_shared<CropImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new CropImageViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNotNull((*vm)->SaveCommand);
        }

        TEST_METHOD(CropImageViewModelShouldSetupCancelCommandWhenConstructed)
        {
            auto vm = std::make_shared<CropImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new CropImageViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNotNull((*vm)->CancelCommand);
        }

        TEST_METHOD(CropImageViewModelShouldSetupResumeCropCommandWhenConstructed)
        {
            auto vm = std::make_shared<CropImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new CropImageViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNotNull((*vm)->ResumeCropCommand);
        }

        TEST_METHOD(CropImageViewModelShouldSetIsAppBarStickyWhenConstructed)
        {
            auto vm = std::make_shared<CropImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new CropImageViewModel(m_repository, m_exceptionPolicy);
            });

            Assert::IsTrue((*vm)->IsAppBarSticky);
        }

        TEST_METHOD(CropImageViewModelCalculateCropOverlayPositionSetsIsCropOverlayVisibleToTrue)
        {
            auto vm = std::make_shared<CropImageViewModel^>(nullptr);
            bool propertyChangedFired = false;

            TestHelper::RunUISynced([this, &propertyChangedFired, vm]() 
            {
                (*vm) = ref new CropImageViewModel(m_repository, m_exceptionPolicy);

                (*vm)->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e) 
                {
                    propertyChangedFired = true;
                });
                GeneralTransform^ gt;

                (*vm)->CalculateInitialCropOverlayPosition(gt, 800.0, 600.0);

            });

            Assert::IsTrue(propertyChangedFired);
            Assert::AreEqual((*vm)->IsCropOverlayVisible, true);
        }

        TEST_METHOD(CropImageViewModelCalculateCropOverlayPositionSetsCorrectCropOverlayWidthAndHeight)
        {
            bool propertyChangedFired = false;
            bool cropOverlayWidthSet = false;
            bool cropOverlayHeightSet = false;

            TestHelper::RunUISynced([this, &propertyChangedFired, &cropOverlayWidthSet, &cropOverlayHeightSet]() 
            {
                auto vm = ref new CropImageViewModel(m_repository, m_exceptionPolicy);
                vm->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e) 
                {
                    propertyChangedFired = true;
                });
                GeneralTransform^ gt;
                vm->CalculateInitialCropOverlayPosition(gt, 800.0, 600.0);
                cropOverlayWidthSet = (vm->CropOverlayWidth == 800.0);
                cropOverlayHeightSet = (vm->CropOverlayHeight == 600.0);
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::IsTrue(cropOverlayWidthSet);
            Assert::IsTrue(cropOverlayHeightSet);
        }

        TEST_METHOD(CropImageViewModelCalculateCropOverlayPositionSetsCorrectCropOverlayLeftAndTop)
        {
            bool propertyChangedFired = false;
            auto vm = std::make_shared<CropImageViewModel^>(nullptr);

            TestHelper::RunUISynced([this, &propertyChangedFired, vm]() 
            {
                (*vm) = ref new CropImageViewModel(m_repository, m_exceptionPolicy);

                (*vm)->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e) 
                {
                    propertyChangedFired = true;
                });
                GeneralTransform^ gt;

                (*vm)->CalculateInitialCropOverlayPosition(gt, 800.0, 600.0);

            });

            Assert::IsTrue(propertyChangedFired);
            Assert::AreEqual((*vm)->CropOverlayLeft, 0.0);
            Assert::AreEqual((*vm)->CropOverlayTop, 0.0);

        }

        TEST_METHOD(CropImageViewModelUpdateCropOverlayPositionUpdatesCropOverlayHeight)
        {
            bool propertyChangedFired = false;
            auto vm = std::make_shared<CropImageViewModel^>(nullptr);

            TestHelper::RunUISynced([this, &propertyChangedFired, vm]() 
            {
                (*vm) = ref new CropImageViewModel(m_repository, m_exceptionPolicy);

                (*vm)->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e) 
                {
                    propertyChangedFired = true;
                });
                GeneralTransform^ gt;
                Thumb^ thumb = ref new Thumb();
                thumb->VerticalAlignment = VerticalAlignment::Bottom;
                (*vm)->CalculateInitialCropOverlayPosition(gt, 800.0, 600.0);
                (*vm)->UpdateCropOverlayPostion(thumb, -2.5, 0.0, 100, 100);

            });

            Assert::IsTrue(propertyChangedFired);
            Assert::AreEqual((*vm)->CropOverlayHeight, 597.5);
        }

        TEST_METHOD(CropImageViewModelUpdateCropOverlayPositionUpdatesCropOverlayHeightAndTop)
        {
            bool propertyChangedFired = false;
            auto vm = std::make_shared<CropImageViewModel^>(nullptr);

            TestHelper::RunUISynced([this, &propertyChangedFired, vm]() 
            {
                (*vm) = ref new CropImageViewModel(m_repository, m_exceptionPolicy);

                (*vm)->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e) 
                {
                    propertyChangedFired = true;
                });
                GeneralTransform^ gt;
                Thumb^ thumb = ref new Thumb();
                thumb->VerticalAlignment = VerticalAlignment::Top;
                (*vm)->CalculateInitialCropOverlayPosition(gt, 800.0, 600.0);
                (*vm)->UpdateCropOverlayPostion(thumb, 2.5, 0.0, 100, 100);

            });

            Assert::IsTrue(propertyChangedFired);
            Assert::AreEqual((*vm)->CropOverlayTop, 2.5);
            Assert::AreEqual((*vm)->CropOverlayHeight, 597.5);
        }

        TEST_METHOD(CropImageViewModelUpdateCropOverlayPositionUpdatesCropOverlayWidth)
        {
            bool propertyChangedFired = false;
            auto vm = std::make_shared<CropImageViewModel^>(nullptr);

            TestHelper::RunUISynced([this, &propertyChangedFired, vm]() 
            {
                (*vm) = ref new CropImageViewModel(m_repository, m_exceptionPolicy);

                (*vm)->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e) 
                {
                    propertyChangedFired = true;
                });
                GeneralTransform^ gt;
                Thumb^ thumb = ref new Thumb();
                thumb->HorizontalAlignment = HorizontalAlignment::Right;
                (*vm)->CalculateInitialCropOverlayPosition(gt, 800.0, 600.0);
                (*vm)->UpdateCropOverlayPostion(thumb, 0.0, -2.5, 100, 100);
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::AreEqual((*vm)->CropOverlayWidth, 797.5);
        }

        TEST_METHOD(CropImageViewModelUpdateCropOverlayPositionUpdatesCropOverlayLeftAndWidth)
        {
            bool propertyChangedFired = false;
            auto vm = std::make_shared<CropImageViewModel^>(nullptr);

            TestHelper::RunUISynced([this, &propertyChangedFired, vm]() 
            {
                (*vm) = ref new CropImageViewModel(m_repository, m_exceptionPolicy);

                (*vm)->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender, PropertyChangedEventArgs^ e) 
                {
                    propertyChangedFired = true;
                });
                GeneralTransform^ gt;
                Thumb^ thumb = ref new Thumb();
                thumb->HorizontalAlignment = HorizontalAlignment::Left;
                (*vm)->CalculateInitialCropOverlayPosition(gt, 800.0, 600.0);
                (*vm)->UpdateCropOverlayPostion(thumb, 0.0, 2.5, 100, 100);
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::AreEqual((*vm)->CropOverlayLeft, 2.5);
            Assert::AreEqual((*vm)->CropOverlayWidth, 797.5);
        }

    private:
        std::shared_ptr<StubRepository> m_repository;
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
    };
}
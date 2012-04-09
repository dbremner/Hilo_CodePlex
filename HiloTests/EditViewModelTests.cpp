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
#include "..\Hilo\EditImageViewModel.h"

using namespace Hilo;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Data;

namespace HiloTests
{
    TEST_CLASS(EditViewModelTests)
    {
    public:

        TEST_METHOD(EditImageViewModelIsNotNull)
        {
            auto vm = ref new EditImageViewModel();

            Assert::IsNotNull(vm, "EditImageViewModel should not be null");
        }

        TEST_METHOD(EditImageViewModelUndoCommandIsNotNull)
        {
            auto vm = ref new EditImageViewModel();
            ICommand^ undoCommand = vm->UndoEditCommand;
            Assert::IsNotNull(undoCommand, "Undo command should not be null");        
        }

        TEST_METHOD(EditImageViewModelCancelCommandIsNotNull)
        {
            auto vm = ref new EditImageViewModel();
            ICommand^ cancelCommand = vm->CancelEditCommand;
            Assert::IsNotNull(cancelCommand, "Cancel command should not be null");        
        }

        TEST_METHOD(EditImageViewModelSaveCommandIsNotNull)
        {
            auto vm = ref new EditImageViewModel();
            ICommand^ saveCommand = vm->SaveEditCommand;
            Assert::IsNotNull(saveCommand, "Save command should not be null");        
        }      

        TEST_METHOD(EditImageViewModelShouldFireOnPropertyChangedWhenSettingAppBarIsSticky)
        {
            auto vm = ref new EditImageViewModel();
            bool propertyChangedFired = false;
            vm->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender,  PropertyChangedEventArgs^ e) {
                propertyChangedFired = true;
            });

            TestHelper::RunUIAsync([vm]() {
                vm->IsAppBarSticky = true;
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::IsTrue(vm->IsAppBarSticky);
        }

        TEST_METHOD(EditImageViewModelRotationAngleCalculationUsingDeltaIsCorrect)
        {
            auto vm = ref new EditImageViewModel();

            TestHelper::RunUIAsync([vm]() {
                vm->CalculateRotationAngle(1);
            });

            Assert::AreEqual(57.295779513082323, vm->Angle);
        }

        TEST_METHOD(EditImageViewModelRotationAngleCalculationUsingDimensionsIsCorrect)
        {
            auto vm = ref new EditImageViewModel();

            TestHelper::RunUIAsync([vm]() {
                vm->CalculateRotationAngle(100.0, 100.0, 500.0, 500.0);
            });

            Assert::AreEqual(224.99999601560978, vm->Angle);
        }

        TEST_METHOD(EditImageViewModelGetCropStartCoordinatesSetsCropRectangleDimensionsToZero)
        {
            auto vm = ref new EditImageViewModel();

            Point p1, p2;
            p1.X = 50;
            p1.Y = 50;
            p2.X = 100;
            p2.Y = 100;
            vm->GetCropStartCoordinates(p1, p2, 140);

            Assert::AreEqual(vm->CropRectangleWidth, 0);
            Assert::AreEqual(vm->CropRectangleHeight, 0);
        }

        TEST_METHOD(EditImageViewModelCalculateCropRectangleCoordinatesSetsIsCropRectangleVisibleToTrue)
        {
            auto vm = ref new EditImageViewModel();
            bool propertyChangedFired = false;
            vm->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender,  PropertyChangedEventArgs^ e) {
                propertyChangedFired = true;
            });

            Point p1;
            p1.X = 50;
            p1.Y = 50;

            Thickness margin;
            margin.Left = 100;
            margin.Top = 100;

            TestHelper::RunUIAsync([vm, p1, margin]() {
                vm->CalculateCropRectangleCoordinates(p1, margin);
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::IsTrue(vm->IsCropRectangleVisible);
        }

        TEST_METHOD(EditImageViewModelCalculateCropRectangleCoordinatesSetsCropRectangleHeightCorrectly)
        {
            auto vm = ref new EditImageViewModel();
            bool propertyChangedFired = false;
            vm->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender,  PropertyChangedEventArgs^ e) {
                propertyChangedFired = true;
            });

            Point p1;
            p1.X = 50;
            p1.Y = 50;

            Thickness margin;
            margin.Left = 100;
            margin.Top = 100;

            TestHelper::RunUIAsync([vm, p1, margin]() {
                vm->CalculateCropRectangleCoordinates(p1, margin);
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::AreEqual(vm->CropRectangleHeight, 50);
        }

        TEST_METHOD(EditImageViewModelCalculateCropRectangleCoordinatesSetsCropRectangleWidthCorrectly)
        {
            auto vm = ref new EditImageViewModel();
            bool propertyChangedFired = false;
            vm->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender,  PropertyChangedEventArgs^ e) {
                propertyChangedFired = true;
            });

            Point p1;
            p1.X = 50;
            p1.Y = 50;

            Thickness margin;
            margin.Left = 100;
            margin.Top = 100;

            TestHelper::RunUIAsync([vm, p1, margin]() {
                vm->CalculateCropRectangleCoordinates(p1, margin);
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::AreEqual(vm->CropRectangleWidth, 50);
        }

    };
}


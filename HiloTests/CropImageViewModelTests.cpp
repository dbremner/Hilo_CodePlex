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

using namespace Hilo;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;

namespace HiloTests
{
	TEST_CLASS(CropImageViewModelTests)
	{
	public:
		
		TEST_METHOD(CropImageViewModelIsNotNull)
		{
            auto vm = ref new CropImageViewModel();
            Assert::IsNotNull(vm, "CropImageViewModel should not be null");
		}

        TEST_METHOD(CropImageViewModelSaveCommandIsNotNull)
        {
            auto vm = ref new CropImageViewModel();
            ICommand^ saveCommand = vm->SaveCommand;
            Assert::IsNotNull(saveCommand, "Save command should not be null");
        }

        TEST_METHOD(CropImageViewModelCancelCommandIsNotNull)
        {
            auto vm = ref new CropImageViewModel();
            ICommand^ cancelCommand = vm->CancelCommand;
            Assert::IsNotNull(cancelCommand, "Cancel command should not be null");
        }
        
        TEST_METHOD(CropImageViewModelGetCropStartCoordinatesSetsCropRectangleDimensionsToZero)
        {
            auto vm = ref new CropImageViewModel();

            Point p1, p2;
            p1.X = 50;
            p1.Y = 50;
            p2.X = 100;
            p2.Y = 100;
            vm->GetCropStartCoordinates(p1, p2, 140);

            Assert::AreEqual(vm->CropRectangleWidth, 0);
            Assert::AreEqual(vm->CropRectangleHeight, 0);
        }
        
        TEST_METHOD(CropImageViewModelCalculateCropRectangleCoordinatesSetsIsCropRectangleVisibleToTrue)
        {
            auto vm = ref new CropImageViewModel();
            bool propertyChangedFired = false;
            vm->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender,  PropertyChangedEventArgs^ e) {
                propertyChangedFired = true;
            });

            Point p1,p2;
            p1.X = 50;
            p1.Y = 50;
            p2.X = 100;
            p2.Y = 100;

            Thickness margin;
            margin.Left = 100;
            margin.Top = 100;

            TestHelper::RunUISynced([vm, p1, p2, margin]() {
                vm->CalculateCropRectangleCoordinates(p1, p2,margin);
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::IsTrue(vm->IsCropRectangleVisible);
        }

        TEST_METHOD(CropImageViewModelCalculateCropRectangleCoordinatesSetsCropRectangleHeightCorrectly)
        {
            auto vm = ref new CropImageViewModel();
            bool propertyChangedFired = false;
            vm->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender,  PropertyChangedEventArgs^ e) {
                propertyChangedFired = true;
            });

            Point p1,p2;
            p1.X = 50;
            p1.Y = 50;
            p2.X = 100;
            p2.Y = 100;

            Thickness margin;
            margin.Left = 100;
            margin.Top = 100;

            TestHelper::RunUISynced([vm, p1, p2, margin]() {
                vm->CalculateCropRectangleCoordinates(p1, p2, margin);
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::AreEqual(vm->CropRectangleHeight, 50);
        }

        TEST_METHOD(CropImageViewModelCalculateCropRectangleCoordinatesSetsCropRectangleWidthCorrectly)
        {
            auto vm = ref new CropImageViewModel();
            bool propertyChangedFired = false;
            vm->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender,  PropertyChangedEventArgs^ e) {
                propertyChangedFired = true;
            });

            Point p1,p2;
            p1.X = 50;
            p1.Y = 50;
            p2.X = 100;
            p2.Y = 100;

            Thickness margin;
            margin.Left = 100;
            margin.Top = 100;

            TestHelper::RunUISynced([vm, p1, p2, margin]() {
                vm->CalculateCropRectangleCoordinates(p1, p2, margin);
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::AreEqual(vm->CropRectangleWidth, 50);
        }

	};
}
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
#include "..\Hilo\RotateImageViewModel.h"
#include <tuple>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Hilo;

namespace HiloTests
{		
    TEST_CLASS(RotateImageViewModelTests)
    {
    public:
        TEST_METHOD(RotateImageViewModelInitialRotationAngleIsZero)
        {
            TestImageGenerator imageGenerator;

            concurrency::task_status status;
            auto imageFile = TestHelper::RunSynced(
                imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png"), 
                status);
            Assert::AreEqual(concurrency::task_status::completed, status);

            RotateImageViewModel^ vm = ref new RotateImageViewModel();

            vm->Initialize(imageFile);

            Assert::AreEqual(0.0, vm->RotationAngle);

            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
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
            std::for_each(begin(testAngles), end(testAngles), [this](std::tuple<double,double> item)
            {
                TestRotation(std::get<0>(item), std::get<1>(item));
            });

        }

        TEST_METHOD(RotateImageViewModelRotateCommandIncrementsBy90)
        {
            TestImageGenerator imageGenerator;
            concurrency::task_status status;
            auto imageFile = TestHelper::RunSynced(
                imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png"), 
                status);
            Assert::AreEqual(concurrency::task_status::completed, status);

            RotateImageViewModel^ vm = ref new RotateImageViewModel();

            vm->Initialize(imageFile);

            TestHelper::RunUISynced([this, vm](){
                vm->RotateCommand->Execute(nullptr);
            });

            Assert::AreEqual(90.0, vm->RotationAngle);

            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
        }

    private:
        void TestRotation(double angle, double expected)
        {
            TestImageGenerator imageGenerator;
            concurrency::task_status status;
            auto imageFile = TestHelper::RunSynced(
                imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile2.png"), 
                status);
            Assert::AreEqual(concurrency::task_status::completed, status);

            RotateImageViewModel^ vm = ref new RotateImageViewModel();

            vm->Initialize(imageFile);

            TestHelper::RunUISynced([this, vm, angle](){
                vm->RotationAngle = angle;
                vm->EndRotation();
            });

            auto message = ref new Platform::String(L"Unexpected result rotating to angle ");
            message += angle.ToString();
            Assert::AreEqual(expected, vm->RotationAngle, message->Data());

            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
        }
    };
}

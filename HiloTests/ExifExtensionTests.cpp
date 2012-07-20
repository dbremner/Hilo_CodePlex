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

#include "..\Hilo\ExifExtensions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Windows::Foundation;

namespace HiloTests
{
    TEST_CLASS(ExifExtensionTests)
    {


    public:

        TEST_METHOD_INITIALIZE(TestSetup)
        {
            errorTolerance = 0.0010f;
        }

        TEST_METHOD(ExifExtensionsTestsShouldTranslateFullRectWhenRotated90)
        {
            Rect item(0.0, 0.0, 25.0, 75.0);

            auto rotated = Hilo::ExifExtensions::RotateClockwise(item, item, 90.0);

            AreEqual(0.0f, rotated.X, errorTolerance);
            AreEqual(0.0f, rotated.Y, errorTolerance);
            AreEqual(75.0f, rotated.Width, errorTolerance);
            AreEqual(25.0f, rotated.Height, errorTolerance);
        }

        TEST_METHOD(ExifExtensionsTestsShouldTranslateFullRectWhenRotated180)
        {
            float errorTolerance = 0.0010f;	

            Rect item(0.0, 0.0, 25.0, 75.0);

            auto rotated = Hilo::ExifExtensions::RotateClockwise(item, item, 180.0);

            AreEqual(0.0f, rotated.X, errorTolerance);
            AreEqual(0.0f, rotated.Y, errorTolerance);
            AreEqual(25.0f, rotated.Width, errorTolerance);
            AreEqual(75.0f, rotated.Height, errorTolerance);
        }

        TEST_METHOD(ExifExtensionsTestsShouldTranslateFullRectWhenRotated270)
        {
            float errorTolerance = 0.0010f;	

            Rect item(0.0, 0.0, 25.0, 75.0);

            auto rotated = Hilo::ExifExtensions::RotateClockwise(item, item, 270.0);

            AreEqual(0.0f, rotated.X, errorTolerance);
            AreEqual(0.0f, rotated.Y, errorTolerance);
            AreEqual(75.0f, rotated.Width, errorTolerance);
            AreEqual(25.0f, rotated.Height, errorTolerance);
        }

        TEST_METHOD(ExifExtensionsTestsShouldRotateRect90RelativeToBitmapDimension)
        {
            float errorTolerance = 0.0010f;	

            Rect originalBitmapDimensions(0.0, 0.0, 100.0, 150.0);
            Rect item(10.0, 10.0, 25.0, 50.0);

            auto rotated = Hilo::ExifExtensions::RotateClockwise(item, originalBitmapDimensions, 90.0);

            AreEqual(90.0f, rotated.X, errorTolerance);
            AreEqual(10.0f, rotated.Y, errorTolerance);
            AreEqual(140.0f, rotated.Right, errorTolerance);
            AreEqual(35.0f, rotated.Bottom, errorTolerance);
        }

        TEST_METHOD(ExifExtensionsTestsShouldRotateRect180RelativeToBitmapDimension)
        {
            float errorTolerance = 0.0010f;	

            Rect originalBitmapDimensions(0.0, 0.0, 100.0, 150.0);
            Rect item(10.0, 10.0, 25.0, 50.0);

            auto rotated = Hilo::ExifExtensions::RotateClockwise(item, originalBitmapDimensions, 180.0);

            AreEqual(65.0f, rotated.X, errorTolerance);
            AreEqual(90.0f, rotated.Y, errorTolerance);
            AreEqual(90.0f, rotated.Right, errorTolerance);
            AreEqual(140.0f, rotated.Bottom, errorTolerance);
        }

        TEST_METHOD(ExifExtensionsTestsShouldRotateRect270RelativeToBitmapDimension)
        {
            float errorTolerance = 0.0010f;	

            Rect originalBitmapDimensions(0.0, 0.0, 100.0, 150.0);
            Rect item(10.0, 10.0, 25.0, 50.0);

            auto rotated = Hilo::ExifExtensions::RotateClockwise(item, originalBitmapDimensions, 270.0);

            AreEqual(10.0f, rotated.X, errorTolerance);
            AreEqual(65.0f, rotated.Y, errorTolerance);
            AreEqual(60.0f, rotated.Right, errorTolerance);
            AreEqual(90.0f, rotated.Bottom, errorTolerance);
        }

        TEST_METHOD(ExifExtensionsTestsRotate270AndMinus90ShouldMatch)
        {
            float errorTolerance = 0.0010f;	

            Rect originalBitmapDimensions(0.0, 0.0, 100.0, 150.0);
            Rect item(10.0, 10.0, 25.0, 50.0);

            auto rotated = Hilo::ExifExtensions::RotateClockwise(item, originalBitmapDimensions, 270.0);
            auto rotated2 = Hilo::ExifExtensions::RotateClockwise(item, originalBitmapDimensions, -90.0);

            Assert::AreEqual(rotated.X, rotated2.X);
            Assert::AreEqual(rotated.Y, rotated2.Y);
            Assert::AreEqual(rotated.Width, rotated2.Width);
            Assert::AreEqual(rotated.Height, rotated2.Height);
        }


    private:
        void AreEqual(float expected, float actual, float tolerance)
        {

            float low = expected - tolerance;
            float high = expected + tolerance;
            if (actual < low || actual > high)
            {
                Assert::Fail(L"Assert equals failed.");
            }
        }

        float errorTolerance;
    };
}


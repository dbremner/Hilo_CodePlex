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
#include "..\Hilo\ImageRotation.h"

using namespace Hilo;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Windows::Foundation;


namespace HiloTests
{
    TEST_CLASS(ImageRotationTests)
    {
    public:

        TEST_METHOD(ImageRotationCalculateRotatedImageSizeIsCorrect)
        {
            auto size = ImageRotation::CalculateRotatedImageSize(36.0, 2048, 1536);

            Assert::AreEqual(safe_cast<int>(size.Width), 2560);
            Assert::AreEqual(safe_cast<int>(size.Height), 2446);
        }

        TEST_METHOD(ImageRotationCalculateRotatedImageSizeIsCorrectWhenAngleIsNegative)
        {
            auto size = ImageRotation::CalculateRotatedImageSize(-45.0, 2048, 1536);

            Assert::AreEqual(safe_cast<int>(size.Width), 2534);
            Assert::AreEqual(safe_cast<int>(size.Height), 2534);
        }

        TEST_METHOD(ImageRotationCalculateRotatedImageSizeIsCorrectWhenAngleIsGreaterThan360)
        {
            auto size = ImageRotation::CalculateRotatedImageSize(376.0, 2048, 1536);

            Assert::AreEqual(safe_cast<int>(size.Width), 2392);
            Assert::AreEqual(safe_cast<int>(size.Height), 2041);
        }

        TEST_METHOD(ImageRotationRotateImageAndCheckThatRotatedPixelsAreNotEqualToSourcePixels)
        {
            int width = 2048;
            int height = 1536;
            int bytesPerPixel = 4;
            double angle = 36.0;

            auto pixels = ref new Array<unsigned char, 1>((width * height) * bytesPerPixel);

            // Initialize pixels array
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    pixels[(x + y * width) * 4] = 0;
                    pixels[(x + y * width) * 4 + 1] = 0;
                    pixels[(x + y * width) * 4 + 2] = 0;
                    pixels[(x + y * width) * 4 + 3] = 0;
                }
            }

            auto size = ImageRotation::CalculateRotatedImageSize(angle, width, height);
            int newWidth = safe_cast<int>(size.Width);
            int newHeight = safe_cast<int>(size.Height);
            auto rotatedPixels = ImageRotation::Rotate(pixels, angle, width, height, newWidth, newHeight);

            Assert::AreNotEqual(pixels, rotatedPixels);
        }

        TEST_METHOD(ImageRotationRotateImageAndCheckThatTheNumberOfRotatedPixelsIsCorrect)
        {
            int width = 2048;
            int height = 1536;
            int bytesPerPixel = 4;
            double angle = 36.0;

            auto pixels = ref new Array<unsigned char, 1>((width * height) * bytesPerPixel);

            // Initialize pixels array
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    pixels[(x + y * width) * 4] = 0;
                    pixels[(x + y * width) * 4 + 1] = 0;
                    pixels[(x + y * width) * 4 + 2] = 0;
                    pixels[(x + y * width) * 4 + 3] = 0;
                }
            }

            auto size = ImageRotation::CalculateRotatedImageSize(angle, width, height);
            int newWidth = safe_cast<int>(size.Width);
            int newHeight = safe_cast<int>(size.Height);
            auto rotatedPixels = ImageRotation::Rotate(pixels, angle, width, height, newWidth, newHeight);

            Assert::AreEqual(safe_cast<int>(rotatedPixels->Length), 25047040);
        }

    };
}
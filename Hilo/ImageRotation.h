//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

namespace Hilo
{
    public ref class ImageRotation sealed
    {
    public:
        static Windows::Foundation::Size CalculateRotatedImageSize(double angle, int width, int height);
        static Platform::Array<unsigned char, 1>^ Rotate(Platform::Array<unsigned char, 1>^ sourcePixels, double angle, int sourceWidth, int sourceHeight, int destWidth, int destHeight);
    };
}


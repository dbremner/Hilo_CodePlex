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
    enum ExifRotations
    {
        NotRotated = 1,
        RotatedDown = 3,
        RotatedLeft = 6,		
        RotatedRight = 8
    };

    class ExifExtensions 
    {

    public:
        static Windows::Foundation::Rect RotateClockwise(Windows::Foundation::Rect rect, Windows::Foundation::Rect bitmapSize, double degrees);
        static unsigned int ConvertExifOrientationToDegreesRotation(unsigned int exifOrientationFlag);
        static unsigned int ConvertDegreesRotationToExifOrientation(unsigned int angle);

    };
}
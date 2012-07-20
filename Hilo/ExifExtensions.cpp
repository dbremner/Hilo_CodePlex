//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ExifExtensions.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

using namespace std;
using namespace Hilo;
using namespace Windows::Foundation;

Rect ExifExtensions::RotateClockwise(Rect rect, Rect bitmapSize, double degrees)
{
    auto radians = (M_PI / 180.0) * degrees;

    auto angleSin = sin(radians);
    auto angleCos = cos(radians);

    auto width = bitmapSize.Width;
    auto height = bitmapSize.Height;

    // calculate rotated translation point for image
    auto x1 = width * angleCos;
    auto y1 = width * angleSin;
    auto x2 = width * angleCos - height*angleSin;
    auto y2 = width * angleSin + height*angleCos;
    auto x3 = -(height * angleSin);
    auto y3 = height * angleCos ;

    auto minX = min(x1, min(x2, x3));
    auto minY = min(y1, min(y2, y3));

    // Adjust rotate and adjust original rect bounding box
    auto xOrigin = (rect.X * angleCos - rect.Y * angleSin) - minX;
    auto yOrigin = (rect.X * angleSin + rect.Y * angleCos) - minY;

    auto xOther = (rect.Right * angleCos - rect.Bottom * angleSin) - minX;
    auto yOther = (rect.Right * angleSin + rect.Bottom * angleCos) - minY;

    Point newOrigin(	static_cast<float>(min(xOrigin, xOther)), 
        static_cast<float>(min(yOrigin, yOther)));
    Point newOther( static_cast<float>(max(xOrigin, xOther)), 
        static_cast<float>(max(yOrigin, yOther)));

    return Rect(newOrigin, newOther);
}

unsigned int ExifExtensions::ConvertExifOrientationToDegreesRotation(unsigned int exifOrientationFlag)
{
    switch (exifOrientationFlag)
    {
    case 1:
        return 0;
    case 6:
        return 90;
    case 3:
        return 180;
    case 8:
        return 270;
    default:
        // Ignore flip/mirroring values (2,4,5,7)
        return 0;
    }
}

unsigned int ExifExtensions::ConvertDegreesRotationToExifOrientation(unsigned int angle)
{
    switch (angle)
    {
    case 0:
        return 1;
    case 90:
        return 6;
    case 180:
        return 3;
    case 270:
        return 8;
    default:
        // Ignore flip/mirroring values (2,4,5,7)
        return 1;
    }

}


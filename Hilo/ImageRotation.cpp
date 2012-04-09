//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ImageRotation.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace Hilo;

using namespace Platform;
using namespace Windows::Foundation;

Size ImageRotation::CalculateRotatedImageSize(double angle, int width, int height)
{
    // Angle's sine and cosine
    double angleRad = angle * M_PI / 180;
    double angleCos = cos(angleRad);
    double angleSin = sin(angleRad);

    // Calculate half size
    double halfWidth = width / 2;
    double halfHeight = height / 2;

    // Rotate corners
    double cornerX1 = halfWidth * angleCos;
    double cornerY1 = halfWidth * angleSin;
    double cornerX2 = halfWidth * angleCos - halfHeight * angleSin;
    double cornerY2 = halfWidth * angleSin + halfHeight * angleCos;
    double cornerX3 = -halfHeight * angleSin;
    double cornerY3 = halfHeight * angleCos;
    double cornerX4 = 0;
    double cornerY4 = 0;

    // Recalculate image size
    halfWidth = max(max(cornerX1, cornerX2), max(cornerX3, cornerX4)) - min(min(cornerX1, cornerX2), min(cornerX3, cornerX4));
    halfHeight = max(max(cornerY1, cornerY2), max(cornerY3, cornerY4)) - min(min(cornerY1, cornerY2), min(cornerY3, cornerY4));

    Size s;
    s.Width = safe_cast<float>(halfWidth * 2 + 0.5);
    s.Height = safe_cast<float>(halfHeight * 2 + 0.5);
    return s;
}

Array<unsigned char, 1>^ ImageRotation::Rotate(Array<unsigned char, 1>^ sourcePixels, double angle, int sourceWidth, int sourceHeight, int destWidth, int destHeight)
{
    // Source image
    double sourceXRadius = (sourceWidth - 1) / 2;
    double sourceYRadius = (sourceHeight - 1) / 2;

    // Allocate the destination pixel array based upon the calculated image size
    auto destinationPixels = ref new Array<unsigned char>((destWidth * destHeight) * 4);

    // Destination image
    double destXRadius = (destWidth - 1) / 2;
    double destYRadius = (destHeight - 1) / 2;

    // Angle's sine and consine
    double angleRad = angle * M_PI / 180;
    double angleCos = cos(angleRad);
    double angleSin = sin(angleRad);

    // Fill values
    byte fillRGB = 0;
    byte fillAlpha = 255;

    // Source pixel's coordinates
    int sourceX, sourceY;

    // Destination pixel's coordinate relative to the image center
    double destCenterX, destCenterY;

    // RGBA nearest neighbour rotation
    destCenterY = -destYRadius;
    for (int y = 0; y < destHeight; y++)
    {
        destCenterX = -destXRadius;
        for (int x = 0; x < destWidth; x++)
        {
            // Coordinate of the nearest point
            sourceX = safe_cast<int>(angleCos * destCenterX + angleSin * destCenterY + sourceXRadius);
            sourceY = safe_cast<int>(-angleSin * destCenterX + angleCos * destCenterY + sourceYRadius);

            // Validate source pixel's coordinates
            if ((sourceX < 0) || (sourceY < 0) || (sourceX >= sourceWidth) || (sourceY >= sourceHeight))
            {
                // Fill destination image with filler
                destinationPixels[(x + y * destWidth) * 4]     = fillRGB;
                destinationPixels[(x + y * destWidth) * 4 + 1] = fillRGB;
                destinationPixels[(x + y * destWidth) * 4 + 2] = fillRGB;
                destinationPixels[(x + y * destWidth) * 4 + 3] = fillAlpha;
            }
            else
            {
                // Fill destination image with pixel from source image
                destinationPixels[(x + y * destWidth) * 4]     = sourcePixels[(sourceX + sourceY * sourceWidth) * 4];
                destinationPixels[(x + y * destWidth) * 4 + 1] = sourcePixels[(sourceX + sourceY * sourceWidth) * 4 + 1];
                destinationPixels[(x + y * destWidth) * 4 + 2] = sourcePixels[(sourceX + sourceY * sourceWidth) * 4 + 2];
                destinationPixels[(x + y * destWidth) * 4 + 3] = sourcePixels[(sourceX + sourceY * sourceWidth) * 4 + 3];
            }
            destCenterX++;
        }
        destCenterY++;
    }
    return destinationPixels;
}
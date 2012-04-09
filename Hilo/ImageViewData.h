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
    ref class ImageViewData sealed
    {
    public:
        ImageViewData(Windows::Storage::BulkAccess::FileInformation^ photo, Platform::Object^ photos);

        property Windows::Storage::BulkAccess::FileInformation^ Photo
        {
            Windows::Storage::BulkAccess::FileInformation^ get();
        }

        property Platform::Object^ Photos
        {
            Platform::Object^ get();
        }

    private:
        Windows::Storage::BulkAccess::FileInformation^ m_photo;
        Platform::Object^ m_photos;
    };
}

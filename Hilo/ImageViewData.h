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
        ImageViewData(Windows::Storage::IStorageFile^ photo, Windows::Storage::IStorageFolder^ folder);

        property Windows::Storage::IStorageFile^ Photo
        {
            Windows::Storage::IStorageFile^ get();
        }

        property Windows::Storage::IStorageFolder^ Folder
        {
            Windows::Storage::IStorageFolder^ get();
        }

    private:
        Windows::Storage::IStorageFile^ m_photo;
        Windows::Storage::IStorageFolder^ m_folder;
    };
}

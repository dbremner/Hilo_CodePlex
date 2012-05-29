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
    ref class Photo;

    ref class ImageViewData sealed
    {
    public:
        ImageViewData(Photo^ photo);

        property Platform::String^ FilePath
        {
            Platform::String^ get();
        }

        property Windows::Foundation::DateTime FileDate
        {
            Windows::Foundation::DateTime get();
        }

        property Platform::String^ DateQuery
        {
            Platform::String^ get();
        }

    private:
        Windows::Foundation::DateTime m_fileDate;
        Platform::String^ m_dateQuery;
        Platform::String^ m_filePath;

        void Foo(Windows::Storage::BulkAccess::IStorageItemInformation^ s, Platform::Object^ e);
    };
}

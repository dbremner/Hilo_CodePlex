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
    class PhotoReader
    {
    public:
        concurrency::task<Windows::Foundation::Collections::IVectorView<Windows::Storage::BulkAccess::FileInformation^>^> GetPhotoGroupAsync(Platform::String^ query, unsigned int maxNumberOfItems = 10);
        concurrency::task<Windows::Foundation::Collections::IVectorView<Windows::Storage::StorageFile^>^> GetPhotoStorageFilesAsync(Platform::String^ query, unsigned int maxNumberOfItems = 25);
        Platform::Object^ GetVirtualizedFiles(Platform::String^ queryString);

    private:
        Windows::Storage::Search::StorageFileQueryResult^ GetFileQuery(Platform::String^ query);
    };
}
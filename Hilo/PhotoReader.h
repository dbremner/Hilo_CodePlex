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
        concurrency::task<Windows::Foundation::Collections::IVectorView<Windows::Storage::BulkAccess::FileInformation^>^> GetPhotosAsync(Platform::String^ query, unsigned int maxNumberOfItems = 10);
        concurrency::task<Windows::Foundation::Collections::IVectorView<Windows::Storage::BulkAccess::FileInformation^>^> GetPhotosAsync(Windows::Storage::Search::IStorageFolderQueryOperations^ folder, Platform::String^ query, unsigned int maxNumberOfItems = 10);
        concurrency::task<Windows::Foundation::Collections::IVectorView<Windows::Storage::StorageFile^>^> GetPhotoStorageFilesAsync(Platform::String^ query, unsigned int maxNumberOfItems = 25);
        Platform::Object^ GetVirtualizedFiles(Platform::String^ queryString);
        Platform::Object^ GetVirtualizedFiles(Windows::Storage::Search::IStorageFolderQueryOperations^ folder, Platform::String^ queryString);
        concurrency::task<Windows::Foundation::Collections::IVectorView<Windows::Storage::BulkAccess::FolderInformation^>^> GetVirtualPhotoFoldersByMonth();

    private:
        Windows::Storage::Search::StorageFolderQueryResult^ CreateVirtualFolderQueryByMonth(Windows::Storage::Search::IStorageFolderQueryOperations^ folder);
        Windows::Storage::Search::StorageFileQueryResult^ CreateFileQuery(Windows::Storage::Search::IStorageFolderQueryOperations^ folder, Platform::String^ query);
    };
}
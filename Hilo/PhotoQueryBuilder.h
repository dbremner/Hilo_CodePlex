//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "PhotoQueryBuilderResult.h"

namespace Hilo
{
    class PhotoQueryBuilder
    {
    public:
        PhotoQueryBuilderResult<Windows::Storage::BulkAccess::FileInformation^> GetAllPhotosAsync(Platform::String^ query, concurrency::cancellation_token token);
        PhotoQueryBuilderResult<Windows::Storage::BulkAccess::FileInformation^> GetAllPhotosAsync(Windows::Storage::Search::IStorageFolderQueryOperations^ folder, Platform::String^ query, concurrency::cancellation_token token);
        PhotoQueryBuilderResult<Windows::Storage::BulkAccess::FileInformation^> GetPhotosAsync(Platform::String^ query, concurrency::cancellation_token token, unsigned int maxNumberOfItems = 10);
        PhotoQueryBuilderResult<Windows::Storage::BulkAccess::FileInformation^> PhotoQueryBuilder::GetPhotoAsync(Platform::String^ photoQuery, concurrency::cancellation_token token);
        PhotoQueryBuilderResult<Windows::Storage::BulkAccess::FileInformation^> GetPhotosAsync(Windows::Storage::Search::IStorageFolderQueryOperations^ folder, Platform::String^ query, concurrency::cancellation_token token, unsigned int maxNumberOfItems = 10);
        concurrency::task<Windows::Foundation::Collections::IVectorView<Windows::Storage::StorageFile^>^> GetPhotoStorageFilesAsync(Platform::String^ query, concurrency::cancellation_token token, unsigned int maxNumberOfItems = 25);
        PhotoQueryBuilderResult<Windows::Storage::BulkAccess::FolderInformation^> GetVirtualPhotoFoldersByMonth(concurrency::cancellation_token token);
        concurrency::task<Windows::Foundation::Collections::IVectorView<Windows::Storage::BulkAccess::FolderInformation^>^> GetVirtualPhotoFoldersByYear(concurrency::cancellation_token token);

    private:
        PhotoQueryBuilderResult<Windows::Storage::BulkAccess::FolderInformation^> GetVirtualPhotoFoldersByMonth(Windows::Storage::Search::IStorageFolderQueryOperations^ folder, concurrency::cancellation_token token);
        Windows::Storage::Search::StorageFolderQueryResult^ CreateVirtualFolderQueryByYear(Windows::Storage::Search::IStorageFolderQueryOperations^ folder);
        Windows::Storage::Search::StorageFolderQueryResult^ CreateVirtualFolderQueryByMonth(Windows::Storage::Search::IStorageFolderQueryOperations^ folder);
        Windows::Storage::Search::StorageFileQueryResult^ CreateFileQuery(Windows::Storage::Search::IStorageFolderQueryOperations^ folder, Platform::String^ query, Windows::Storage::Search::IndexerOption option = Windows::Storage::Search::IndexerOption::UseIndexerWhenAvailable);
    };
}
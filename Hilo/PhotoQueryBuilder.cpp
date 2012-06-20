//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "PhotoQueryBuilder.h"
#include "TaskExtensions.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Search;

const std::array<String^, 6> items = { ".jpg", ".jpeg", ".png", ".bmp", ".gif", ".tif" };

PhotoQueryBuilderResult<Windows::Storage::BulkAccess::FileInformation^> PhotoQueryBuilder::GetAllPhotosAsync(Platform::String^ query)
{
    auto fileQuery = CreateFileQuery(KnownFolders::PicturesLibrary, query);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return PhotoQueryBuilderResult<FileInformation^>(create_task(fileInformationFactory->GetFilesAsync()), fileQuery);
}

PhotoQueryBuilderResult<FileInformation^> PhotoQueryBuilder::GetAllPhotosAsync(Windows::Storage::Search::IStorageFolderQueryOperations^ folder, Platform::String^ query)
{
    auto fileQuery = CreateFileQuery(folder, query);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return PhotoQueryBuilderResult<FileInformation^>(create_task(fileInformationFactory->GetFilesAsync()), fileQuery);
}

PhotoQueryBuilderResult<FileInformation^> PhotoQueryBuilder::GetPhotosAsync(String^ query, unsigned int maxNumberOfItems)
{
    auto fileQuery = CreateFileQuery(KnownFolders::PicturesLibrary, query);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return PhotoQueryBuilderResult<FileInformation^>(create_task(fileInformationFactory->GetFilesAsync(0, maxNumberOfItems)), fileQuery);
}

PhotoQueryBuilderResult<FileInformation^> PhotoQueryBuilder::GetPhotosAsync(IStorageFolderQueryOperations^ folder, String^ query, unsigned int maxNumberOfItems)
{
    auto fileQuery = CreateFileQuery(folder, query);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return PhotoQueryBuilderResult<FileInformation^>(create_task(fileInformationFactory->GetFilesAsync(0, maxNumberOfItems)), fileQuery);
}

//task<IVectorView<FileInformation^>^> PhotoQueryBuilder::GetPhotosAsync(String^ query, unsigned int maxNumberOfItems)
//{
//    auto fileQuery = CreateFileQuery(KnownFolders::PicturesLibrary, query);
//    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
//    return create_task(fileInformationFactory->GetFilesAsync(0, maxNumberOfItems));
//}
//
//task<IVectorView<FileInformation^>^> PhotoQueryBuilder::GetPhotosAsync(IStorageFolderQueryOperations^ folder, String^ query, unsigned int maxNumberOfItems)
//{
//    auto fileQuery = CreateFileQuery(folder, query);
//    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
//    return create_task(fileInformationFactory->GetFilesAsync(0, maxNumberOfItems));
//}

task<IVectorView<StorageFile^>^> PhotoQueryBuilder::GetPhotoStorageFilesAsync(String^ query, unsigned int maxNumberOfItems)
{
    auto fileQuery = CreateFileQuery(KnownFolders::PicturesLibrary, query);
    return create_task(fileQuery->GetFilesAsync(0, maxNumberOfItems));
}

task<IVectorView<FolderInformation^>^> PhotoQueryBuilder::GetVirtualPhotoFoldersByMonth()
{
    return GetVirtualPhotoFoldersByMonth(KnownFolders::PicturesLibrary);
}

task<IVectorView<FolderInformation^>^> PhotoQueryBuilder::GetVirtualPhotoFoldersByMonth(IStorageFolderQueryOperations^ folder)
{
    auto fileQuery = CreateVirtualFolderQueryByMonth(folder);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return create_task(fileInformationFactory->GetFoldersAsync());
}

task<IVectorView<FolderInformation^>^> PhotoQueryBuilder::GetVirtualPhotoFoldersByYear()
{
    auto fileQuery = CreateVirtualFolderQueryByYear(KnownFolders::PicturesLibrary);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return create_task(fileInformationFactory->GetFoldersAsync());
}

StorageFolderQueryResult^ PhotoQueryBuilder::CreateVirtualFolderQueryByYear(IStorageFolderQueryOperations^ folder)
{
    auto queryOptions = ref new QueryOptions(CommonFolderQuery::GroupByYear);
    queryOptions->FolderDepth = FolderDepth::Deep;
    queryOptions->IndexerOption = IndexerOption::UseIndexerWhenAvailable;
    return folder->CreateFolderQueryWithOptions(queryOptions);
}

StorageFolderQueryResult^ PhotoQueryBuilder::CreateVirtualFolderQueryByMonth(IStorageFolderQueryOperations^ folder)
{
    auto queryOptions = ref new QueryOptions(CommonFolderQuery::GroupByMonth);
    queryOptions->FolderDepth = FolderDepth::Deep;
    queryOptions->IndexerOption = IndexerOption::UseIndexerWhenAvailable;
    return folder->CreateFolderQueryWithOptions(queryOptions);
}

StorageFileQueryResult^ PhotoQueryBuilder::CreateFileQuery(IStorageFolderQueryOperations^ folder, String^ query)
{
    auto fileTypeFilter = ref new Vector<String^>(items);

    //auto picturesFolder = KnownFolders::PicturesLibrary;
    auto queryOptions = ref new QueryOptions(CommonFileQuery::OrderByDate, fileTypeFilter);
    queryOptions->FolderDepth = FolderDepth::Deep;
    queryOptions->IndexerOption = IndexerOption::UseIndexerWhenAvailable;
    queryOptions->ApplicationSearchFilter = query;
    queryOptions->SetThumbnailPrefetch(ThumbnailMode::PicturesView, 190, ThumbnailOptions::UseCurrentScale);
    return folder->CreateFileQueryWithOptions(queryOptions);
}

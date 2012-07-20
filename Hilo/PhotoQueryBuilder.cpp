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

PhotoQueryBuilderResult<Windows::Storage::BulkAccess::FileInformation^> PhotoQueryBuilder::GetAllPhotosAsync(Platform::String^ query, cancellation_token token)
{
    auto fileQuery = CreateFileQuery(KnownFolders::PicturesLibrary, query);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return PhotoQueryBuilderResult<FileInformation^>(create_task(fileInformationFactory->GetFilesAsync(), token), fileQuery);
}

PhotoQueryBuilderResult<FileInformation^> PhotoQueryBuilder::GetAllPhotosAsync(Windows::Storage::Search::IStorageFolderQueryOperations^ folder, Platform::String^ query, cancellation_token token)
{
    auto fileQuery = CreateFileQuery(folder, query);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return PhotoQueryBuilderResult<FileInformation^>(create_task(fileInformationFactory->GetFilesAsync(), token), fileQuery);
}

PhotoQueryBuilderResult<FileInformation^> PhotoQueryBuilder::GetPhotosAsync(String^ query, cancellation_token token, unsigned int maxNumberOfItems)
{
    auto fileQuery = CreateFileQuery(KnownFolders::PicturesLibrary, query);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return PhotoQueryBuilderResult<FileInformation^>(create_task(fileInformationFactory->GetFilesAsync(0, maxNumberOfItems), token), fileQuery);
}

PhotoQueryBuilderResult<FileInformation^> PhotoQueryBuilder::GetPhotoAsync(String^ photoQuery, cancellation_token token)
{
    String^ query = "System.ParsingPath:=\"" + photoQuery + "\"";
    
    auto fileQuery = CreateFileQuery(KnownFolders::PicturesLibrary, query, IndexerOption::DoNotUseIndexer);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return PhotoQueryBuilderResult<FileInformation^>(create_task(fileInformationFactory->GetFilesAsync(0, 1), token), fileQuery);
}

PhotoQueryBuilderResult<FileInformation^> PhotoQueryBuilder::GetPhotosAsync(IStorageFolderQueryOperations^ folder, String^ query, cancellation_token token, unsigned int maxNumberOfItems)
{
    auto fileQuery = CreateFileQuery(folder, query);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return PhotoQueryBuilderResult<FileInformation^>(create_task(fileInformationFactory->GetFilesAsync(0, maxNumberOfItems), token), fileQuery);
}

task<IVectorView<StorageFile^>^> PhotoQueryBuilder::GetPhotoStorageFilesAsync(String^ query, cancellation_token token, unsigned int maxNumberOfItems)
{
    auto fileQuery = CreateFileQuery(KnownFolders::PicturesLibrary, query);
    return create_task(fileQuery->GetFilesAsync(0, maxNumberOfItems), token);
}

PhotoQueryBuilderResult<FolderInformation^> PhotoQueryBuilder::GetVirtualPhotoFoldersByMonth(cancellation_token token)
{
    return GetVirtualPhotoFoldersByMonth(KnownFolders::PicturesLibrary, token);
}

PhotoQueryBuilderResult<FolderInformation^> PhotoQueryBuilder::GetVirtualPhotoFoldersByMonth(IStorageFolderQueryOperations^ folder, cancellation_token token)
{
    auto fileQuery = CreateVirtualFolderQueryByMonth(folder);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return PhotoQueryBuilderResult<FolderInformation^>(create_task(fileInformationFactory->GetFoldersAsync(), token), fileQuery);
}

task<IVectorView<FolderInformation^>^> PhotoQueryBuilder::GetVirtualPhotoFoldersByYear(cancellation_token token)
{
    auto fileQuery = CreateVirtualFolderQueryByYear(KnownFolders::PicturesLibrary);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return create_task(fileInformationFactory->GetFoldersAsync(), token);
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

StorageFileQueryResult^ PhotoQueryBuilder::CreateFileQuery(IStorageFolderQueryOperations^ folder, String^ query, IndexerOption indexerOption)
{
    auto fileTypeFilter = ref new Vector<String^>(items);

    //auto picturesFolder = KnownFolders::PicturesLibrary;
    auto queryOptions = ref new QueryOptions(CommonFileQuery::OrderByDate, fileTypeFilter);
    queryOptions->FolderDepth = FolderDepth::Deep;
    queryOptions->IndexerOption = indexerOption;
    queryOptions->ApplicationSearchFilter = query;
    queryOptions->SetThumbnailPrefetch(ThumbnailMode::PicturesView, 190, ThumbnailOptions::UseCurrentScale);
    return folder->CreateFileQueryWithOptions(queryOptions);
}

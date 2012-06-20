//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "PhotoReader.h"
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

PhotoReaderResult<FileInformation^> PhotoReader::GetPhotosAsyncNew(String^ query, unsigned int maxNumberOfItems)
{
    auto fileQuery = CreateFileQuery(KnownFolders::PicturesLibrary, query);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return PhotoReaderResult<FileInformation^>(create_task(fileInformationFactory->GetFilesAsync(0, maxNumberOfItems)), fileQuery);
}

task<IVectorView<FileInformation^>^> PhotoReader::GetPhotosAsync(String^ query, unsigned int maxNumberOfItems)
{
    auto fileQuery = CreateFileQuery(KnownFolders::PicturesLibrary, query);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return create_task(fileInformationFactory->GetFilesAsync(0, maxNumberOfItems));
}

task<IVectorView<FileInformation^>^> PhotoReader::GetPhotosAsync(IStorageFolderQueryOperations^ folder, String^ query, unsigned int maxNumberOfItems)
{
    auto fileQuery = CreateFileQuery(folder, query);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return create_task(fileInformationFactory->GetFilesAsync(0, maxNumberOfItems));
}

task<IVectorView<FileInformation^>^> PhotoReader::GetAllPhotosAsync(String^ query)
{
    auto fileQuery = CreateFileQuery(KnownFolders::PicturesLibrary, query);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return create_task(fileInformationFactory->GetFilesAsync());
}

task<IVectorView<FileInformation^>^> PhotoReader::GetAllPhotosAsync(IStorageFolderQueryOperations^ folder, String^ query)
{
    auto fileQuery = CreateFileQuery(folder, query);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return create_task(fileInformationFactory->GetFilesAsync());
}

task<IVectorView<StorageFile^>^> PhotoReader::GetPhotoStorageFilesAsync(String^ query, unsigned int maxNumberOfItems)
{
    auto fileQuery = CreateFileQuery(KnownFolders::PicturesLibrary, query);
    return create_task(fileQuery->GetFilesAsync(0, maxNumberOfItems));
}

Object^ PhotoReader::GetVirtualizedFiles(Platform::String^ queryString)
{
    return GetVirtualizedFiles(KnownFolders::PicturesLibrary, queryString);
}

Object^ PhotoReader::GetVirtualizedFiles(IStorageFolderQueryOperations^ folder, Platform::String^ queryString)
{
    auto fileQuery = CreateFileQuery(folder, queryString);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return fileInformationFactory->GetVirtualizedFilesVector();
}

task<IVectorView<FolderInformation^>^> PhotoReader::GetVirtualPhotoFoldersByMonth()
{
    return GetVirtualPhotoFoldersByMonth(KnownFolders::PicturesLibrary);
}

task<IVectorView<FolderInformation^>^> PhotoReader::GetVirtualPhotoFoldersByMonth(IStorageFolderQueryOperations^ folder)
{
    auto fileQuery = CreateVirtualFolderQueryByMonth(folder);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return create_task(fileInformationFactory->GetFoldersAsync());
}

task<IVectorView<FolderInformation^>^> PhotoReader::GetVirtualPhotoFoldersByYear()
{
    auto fileQuery = CreateVirtualFolderQueryByYear(KnownFolders::PicturesLibrary);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView);
    return create_task(fileInformationFactory->GetFoldersAsync());
}

StorageFolderQueryResult^ PhotoReader::CreateVirtualFolderQueryByYear(IStorageFolderQueryOperations^ folder)
{
    auto queryOptions = ref new QueryOptions(CommonFolderQuery::GroupByYear);
    queryOptions->FolderDepth = FolderDepth::Deep;
    queryOptions->IndexerOption = IndexerOption::UseIndexerWhenAvailable;
    return folder->CreateFolderQueryWithOptions(queryOptions);
}

StorageFolderQueryResult^ PhotoReader::CreateVirtualFolderQueryByMonth(IStorageFolderQueryOperations^ folder)
{
    auto queryOptions = ref new QueryOptions(CommonFolderQuery::GroupByMonth);
    queryOptions->FolderDepth = FolderDepth::Deep;
    queryOptions->IndexerOption = IndexerOption::UseIndexerWhenAvailable;
    return folder->CreateFolderQueryWithOptions(queryOptions);
}

StorageFileQueryResult^ PhotoReader::CreateFileQuery(IStorageFolderQueryOperations^ folder, String^ query)
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

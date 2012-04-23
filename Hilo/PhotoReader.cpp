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

task<IVectorView<FileInformation^>^> PhotoReader::GetPhotosAsync(String^ query, unsigned int maxNumberOfItems)
{
    auto fileQuery = CreateFileQuery(KnownFolders::PicturesLibrary, query);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView, 200, ThumbnailOptions::UseCurrentScale , false);
    return task<IVectorView<FileInformation^>^>(fileInformationFactory->GetFilesAsync(0, maxNumberOfItems));
}

task<IVectorView<FileInformation^>^> PhotoReader::GetPhotosAsync(IStorageFolderQueryOperations^ folder, String^ query, unsigned int maxNumberOfItems)
{
    auto fileQuery = CreateFileQuery(folder, query);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView, 200, ThumbnailOptions::UseCurrentScale , false);
    return task<IVectorView<FileInformation^>^>(fileInformationFactory->GetFilesAsync(0, maxNumberOfItems));
}

task<IVectorView<StorageFile^>^> PhotoReader::GetPhotoStorageFilesAsync(String^ query, unsigned int maxNumberOfItems)
{
    auto fileQuery = CreateFileQuery(KnownFolders::PicturesLibrary, query);
    return concurrency::task<IVectorView<StorageFile^>^>(fileQuery->GetFilesAsync(0, maxNumberOfItems));
}

Object^ PhotoReader::GetVirtualizedFiles(Platform::String^ queryString)
{
    return GetVirtualizedFiles(KnownFolders::PicturesLibrary, queryString);
}

Object^ PhotoReader::GetVirtualizedFiles(IStorageFolderQueryOperations^ folder, Platform::String^ queryString)
{
    auto fileQuery = CreateFileQuery(folder, queryString);
    
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView, 200, ThumbnailOptions::UseCurrentScale , false);
    return fileInformationFactory->GetVirtualizedFilesVector();
}

task<IVectorView<FolderInformation^>^> PhotoReader::GetVirtualPhotoFoldersByMonth()
{
    auto fileQuery = CreateVirtualFolderQueryByMonth(KnownFolders::PicturesLibrary);

    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView, 200, ThumbnailOptions::UseCurrentScale , false);
    return task<IVectorView<FolderInformation^>^>(fileInformationFactory->GetFoldersAsync());
}

StorageFolderQueryResult^ PhotoReader::CreateVirtualFolderQueryByMonth(IStorageFolderQueryOperations^ folder)
{
    auto queryOptions = ref new QueryOptions(CommonFolderQuery::GroupByMonth);
    queryOptions->FolderDepth = FolderDepth::Deep;
    queryOptions->IndexerOption = IndexerOption::DoNotUseIndexer;
    return folder->CreateFolderQueryWithOptions(queryOptions);
}

StorageFileQueryResult^ PhotoReader::CreateFileQuery(IStorageFolderQueryOperations^ folder, String^ query)
{
    auto fileTypeFilter = ref new Vector<String^>(items);

    //auto picturesFolder = KnownFolders::PicturesLibrary;
    auto queryOptions = ref new QueryOptions(CommonFileQuery::OrderByDate, fileTypeFilter);
    queryOptions->FolderDepth = FolderDepth::Deep;
    queryOptions->IndexerOption = IndexerOption::DoNotUseIndexer;
    queryOptions->ApplicationSearchFilter = query;
    return folder->CreateFileQueryWithOptions(queryOptions);
    
}

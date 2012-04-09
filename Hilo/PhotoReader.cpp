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

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Search;

const std::array<String^, 6> items = { ".jpg", ".jpeg", ".png", ".bmp", ".gif", ".tif" };

concurrency::task<IVectorView<FileInformation^>^> PhotoReader::GetPhotoGroupAsync(String^ query, unsigned int maxNumberOfItems)
{
    auto fileQuery = GetFileQuery(query);
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView, 200, ThumbnailOptions::UseCurrentScale , false);
    return concurrency::task<IVectorView<FileInformation^>^>(fileInformationFactory->GetFilesAsync(0, maxNumberOfItems));
}

concurrency::task<IVectorView<StorageFile^>^> PhotoReader::GetPhotoStorageFilesAsync(String^ query, unsigned int maxNumberOfItems)
{
    auto fileQuery = GetFileQuery(query);
    return concurrency::task<IVectorView<StorageFile^>^>(fileQuery->GetFilesAsync(0, maxNumberOfItems));
}

Object^ PhotoReader::GetVirtualizedFiles(Platform::String^ queryString)
{
    auto fileQuery = GetFileQuery(queryString);
    
    auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView, 200, ThumbnailOptions::UseCurrentScale , false);
    return fileInformationFactory->GetVirtualizedFilesVector();
}

StorageFileQueryResult^ PhotoReader::GetFileQuery(String^ query)
{
    auto fileTypeFilter = ref new Vector<String^>(items);

    auto picturesFolder = KnownFolders::PicturesLibrary;
    auto queryOptions = ref new QueryOptions(CommonFileQuery::OrderByDate, fileTypeFilter);
    queryOptions->FolderDepth = FolderDepth::Deep;
    queryOptions->IndexerOption = IndexerOption::UseIndexerWhenAvailable;
    queryOptions->ApplicationSearchFilter = query;
    return picturesFolder->CreateFileQueryWithOptions(queryOptions);
    
}

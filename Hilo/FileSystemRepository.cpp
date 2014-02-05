// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "FileSystemRepository.h"
#include "CalendarExtensions.h"
#include "NullPhotoGroup.h"
#include "Photo.h"
#include "PhotoImage.h"
#include "QueryChange.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace std;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Search;

// See http://go.microsoft.com/fwlink/?LinkId=267277 for info about the FileSystemRepository class.

#pragma region File System Queries

const std::array<String^, 6> items = { ".jpg", ".jpeg", ".png", ".bmp", ".gif", ".tif" };

inline task<IVectorView<StorageFile^>^> FileSystemRepository::GetPhotoStorageFilesAsync(String^ query, unsigned int maxNumberOfItems)
{
    auto fileQuery = CreateFileQuery(KnownFolders::PicturesLibrary, query);
    return create_task(fileQuery->GetFilesAsync(0, maxNumberOfItems));
}

inline StorageFileQueryResult^ FileSystemRepository::CreateFileQuery(IStorageFolderQueryOperations^ folder, String^ query, IndexerOption indexerOption)
{
    auto fileTypeFilter = ref new Vector<String^>(items);
    auto queryOptions = ref new QueryOptions(CommonFileQuery::OrderByDate, fileTypeFilter);
    queryOptions->FolderDepth = FolderDepth::Deep;
    queryOptions->IndexerOption = indexerOption;
    queryOptions->ApplicationSearchFilter = query;
    queryOptions->SetThumbnailPrefetch(ThumbnailMode::PicturesView, 190, ThumbnailOptions::UseCurrentScale);
    queryOptions->Language = CalendarExtensions::ResolvedLanguage();
    return folder->CreateFileQueryWithOptions(queryOptions);
}

#pragma endregion

FileSystemRepository::FileSystemRepository(shared_ptr<ExceptionPolicy> exceptionPolicy) : m_exceptionPolicy(exceptionPolicy)
{
}

#pragma region ObservableQuery Methods
void FileSystemRepository::AddObserver(const std::function<void()> callback, PageType pageType)
{
    switch (pageType)
    {
    case PageType::Hub:
        m_hubViewModelCallback = callback;
        break;
    case PageType::Browse:
        m_imageBrowserViewModelCallback = callback;
        break;
    case PageType::Image:
        m_imageViewModelCallback = callback;
        break;
    default:
        assert(false);
    }
}

void FileSystemRepository::RemoveObserver(PageType pageType)
{
    switch (pageType)
    {
    case PageType::Hub:
        m_pictureHubGroupQueryChange = nullptr;
        m_hubViewModelCallback = nullptr;
        break;
    case PageType::Browse:
        m_monthQueryChange = nullptr;
        m_imageBrowserViewModelCallback = nullptr;
        break;
    case PageType::Image:
        m_allPhotosQueryChange = nullptr;
        m_imageViewModelCallback = nullptr;
        break;
    default:
        assert(false);
    }
}  
#pragma endregion


void FileSystemRepository::NotifyAllObservers()
{
    if (m_hubViewModelCallback != nullptr)
    {
        m_hubViewModelCallback();
    }
    if (m_imageBrowserViewModelCallback != nullptr)
    {
        m_imageBrowserViewModelCallback();
    }
    if (m_imageViewModelCallback != nullptr)
    {
        m_imageViewModelCallback();
    }
}

// Create month groups by querying the file system.
task<IVectorView<IPhotoGroup^>^> FileSystemRepository::GetMonthGroupedPhotosWithCacheAsync(shared_ptr<PhotoCache> photoCache, concurrency::cancellation_token token)
{
    auto queryOptions = ref new QueryOptions(CommonFolderQuery::GroupByMonth);
    queryOptions->FolderDepth = FolderDepth::Deep;
    queryOptions->IndexerOption = IndexerOption::UseIndexerWhenAvailable;
    queryOptions->Language = CalendarExtensions::ResolvedLanguage();
    auto fileQuery = KnownFolders::PicturesLibrary->CreateFolderQueryWithOptions(queryOptions);
    m_monthQueryChange = (m_imageBrowserViewModelCallback != nullptr) ? ref new QueryChange(fileQuery, m_imageBrowserViewModelCallback) : nullptr;

    shared_ptr<ExceptionPolicy> policy = m_exceptionPolicy;
    auto sharedThis = shared_from_this();
    return create_task(fileQuery->GetFoldersAsync()).then([this, photoCache, sharedThis, policy](IVectorView<StorageFolder^>^ folders) 
    {
        auto temp = ref new Vector<IPhotoGroup^>();
        for (auto folder : folders)
        {
            auto photoGroup = ref new MonthGroup(photoCache, folder, sharedThis, policy);
            temp->Append(photoGroup);
        }
        return temp->GetView();
    }, token);
}

// Query the file system using a pathname. Returns a model object that supports full-image display.
task<IPhotoImage^> FileSystemRepository::GetSinglePhotoAsync(String^ photoPath)
{
    assert(IsMainThread());
    String^ query = "System.ParsingPath:=\"" + photoPath + "\"";    
    auto fileQuery = CreateFileQuery(KnownFolders::PicturesLibrary, query, IndexerOption::DoNotUseIndexer);
    shared_ptr<ExceptionPolicy> policy = m_exceptionPolicy;
    return create_task(fileQuery->GetFilesAsync(0, 1)).then([policy](IVectorView<StorageFile^>^ files) -> IPhotoImage^
    {
        if (files->Size > 0)
        {
            IPhotoImage^ photo = (ref new Photo(files->GetAt(0), ref new NullPhotoGroup(), policy))->GetPhotoImage();
            create_task(photo->InitializeAsync());
            return photo;
        }
        else
        {
            return nullptr;
        }
    }, task_continuation_context::use_current());
}

// Count the number of photos in a given folder query.
task<unsigned int> FileSystemRepository::GetFolderPhotoCountAsync(IStorageFolderQueryOperations^ folderQuery)
{
    auto fileTypeFilter = ref new Vector<String^>(items);
    auto queryOptions = ref new QueryOptions(CommonFileQuery::OrderByDate, fileTypeFilter);  // ordered query allows use of indexer
    queryOptions->FolderDepth = FolderDepth::Deep;
    queryOptions->IndexerOption = IndexerOption::UseIndexerWhenAvailable;
    queryOptions->Language = CalendarExtensions::ResolvedLanguage();
    auto fileQuery = folderQuery->CreateFileQueryWithOptions(queryOptions);
    return create_task(fileQuery->GetFilesAsync(0, 1)).then([fileQuery](IVectorView<StorageFile^>^ files)
    {
        return fileQuery->GetItemCountAsync();
    });
}

// Retrieves a given number of photos from a folder query.
task<IVectorView<IPhoto^>^> FileSystemRepository::GetPhotoDataForMonthGroup(IPhotoGroup^ photoGroup, IStorageFolderQueryOperations^ folderQuery, unsigned int maxNumberOfItems)
{
    auto fileQuery = CreateFileQuery(folderQuery, "");
    shared_ptr<ExceptionPolicy> policy = m_exceptionPolicy;
    return create_task(fileQuery->GetFilesAsync(0, maxNumberOfItems)).then([photoGroup, policy, maxNumberOfItems](IVectorView<StorageFile^>^ files) 
    {
        auto photos = ref new Vector<IPhoto^>();
        for (auto item : files)
        {
            auto photo = ref new Photo(item, photoGroup, policy);
            photos->Append(photo);
        }
        return photos->GetView();
    }, task_continuation_context::use_current());
}

// Returns true if there are any photos in the given folder within the specified date range. Returns false otherwise.
task<bool> FileSystemRepository::HasPhotosInRangeAsync(Platform::String^ dateRangeQuery, IStorageFolderQueryOperations^ folderQuery)
{
    auto fileTypeFilter = ref new Vector<String^>(items);
    auto queryOptions = ref new QueryOptions(CommonFileQuery::OrderByDate, fileTypeFilter);  // ordered query allows use of indexer
    queryOptions->FolderDepth = FolderDepth::Deep;
    queryOptions->IndexerOption = IndexerOption::UseIndexerWhenAvailable;
    queryOptions->Language = CalendarExtensions::ResolvedLanguage();
    queryOptions->ApplicationSearchFilter = dateRangeQuery;
    auto fileQuery = folderQuery->CreateFileQueryWithOptions(queryOptions);
    return create_task(fileQuery->GetFilesAsync(0, 1)).then([](IVectorView<StorageFile^>^ files)
    {
        return (files->Size > 0);
    });
}

// Retrieves the photos from the user's picture library with dates in a given range.
task<IVectorView<IPhoto^>^> FileSystemRepository::GetPhotosForDateRangeQueryAsync(String^ dateRangeQuery)
{
    auto fileQuery = CreateFileQuery(KnownFolders::PicturesLibrary, dateRangeQuery);
    shared_ptr<ExceptionPolicy> policy = m_exceptionPolicy;

    m_allPhotosQueryChange = (m_imageViewModelCallback != nullptr) ? ref new QueryChange(fileQuery, m_imageViewModelCallback) : nullptr;

    return create_task(fileQuery->GetFilesAsync()).then([policy](IVectorView<StorageFile^>^ files) 
    {
        auto photos = ref new Vector<IPhoto^>();
        for (auto file : files)
        {
            auto photo = ref new Photo(file, ref new NullPhotoGroup(), policy);
            photos->Append(photo);
        }
        return photos->GetView();
    }, task_continuation_context::use_current());
}

// Selects a set of recent photos.
task<IVectorView<IPhoto^>^> FileSystemRepository::GetPhotosForPictureHubGroupAsync(IPhotoGroup^ photoGroup, unsigned int maxNumberOfItems)
{
    auto fileQuery = CreateFileQuery(KnownFolders::PicturesLibrary, "");
    m_pictureHubGroupQueryChange = (m_hubViewModelCallback != nullptr) ? ref new QueryChange(fileQuery, m_hubViewModelCallback) : nullptr;
    shared_ptr<ExceptionPolicy> policy = m_exceptionPolicy;
    return create_task(fileQuery->GetFilesAsync(0, maxNumberOfItems)).then([photoGroup, policy](IVectorView<StorageFile^>^ files) 
    {
        auto photos = ref new Vector<IPhoto^>();
        for (auto item : files)
        {
            auto photo = ref new Photo(item, photoGroup, policy);
            photos->Append(photo);
        }
        return photos->GetView();

    }, task_continuation_context::use_current());
}

// Queries the file system for photos grouped by year. There will be one year group for each year that has photos.
task<IVectorView<IYearGroup^>^> FileSystemRepository::GetYearGroupedMonthsAsync(cancellation_token token)
{
    shared_ptr<ExceptionPolicy> policy = m_exceptionPolicy;
    auto queryOptions = ref new QueryOptions(CommonFolderQuery::GroupByYear);
    queryOptions->FolderDepth = FolderDepth::Deep;
    queryOptions->IndexerOption = IndexerOption::UseIndexerWhenAvailable;
    queryOptions->Language = CalendarExtensions::ResolvedLanguage();
    auto fileQuery =  KnownFolders::PicturesLibrary->CreateFolderQueryWithOptions(queryOptions);
    return create_task(fileQuery->GetFoldersAsync()).then([this, policy](IVectorView<StorageFolder^>^ folders) 
    {
        vector<task<YearGroup^>> yearGroupTasks;
        for (auto folder : folders)
        {
            yearGroupTasks.push_back(GetYearGroupAsync(folder, policy));
        }
        return when_all(begin(yearGroupTasks), end(yearGroupTasks)).then([](vector<YearGroup^> groups)
        {
            auto result = ref new Vector<IYearGroup^>();
            for (auto group : groups)
            {
                if (group != nullptr)
                {
                    result->Append(group);
                }
            }
            return result->GetView();
        });
    }, token, task_continuation_context::use_arbitrary());
}

// Returns an image file's DateTaken, or if none, the file's DateModified
// The result is the same as "System.ItemDate" for image files.
static task<DateTime> GetFileDateTimeAsync(StorageFile^ file)
{
   return create_task(file->Properties->GetImagePropertiesAsync()).then([file](ImageProperties^ imageProperties) -> task<DateTime>
   {
      auto dateTaken = imageProperties->DateTaken;
      if (dateTaken.UniversalTime == 0) 
      {
        return create_task(file->GetBasicPropertiesAsync()).then([](BasicProperties^ basicProperties) 
        { 
           return basicProperties->DateModified;
        });
      }
      else
      {
        return create_task_from_result<DateTime>(dateTaken);
      }
    }, task_continuation_context::use_arbitrary());
}            

// Creates a YearGroup object for each folder created by the year query.
task<YearGroup^> FileSystemRepository::GetYearGroupAsync(IStorageFolderQueryOperations^ folderQuery, shared_ptr<ExceptionPolicy> exceptionPolicy)
{
    auto repository = shared_from_this();
    auto fileTypeFilter = ref new Vector<String^>(items);
    auto queryOptions = ref new QueryOptions(CommonFileQuery::OrderByDate, fileTypeFilter);
    queryOptions->IndexerOption = IndexerOption::UseIndexerWhenAvailable;
    queryOptions->Language = CalendarExtensions::ResolvedLanguage();
    auto fileQuery = folderQuery->CreateFileQueryWithOptions(queryOptions);
    auto maxNumberOfItems = 1;
   
    return create_task(fileQuery->GetFilesAsync(0, maxNumberOfItems)).then([](task<IVectorView<StorageFile^>^> filesTask) -> task<DateTime>
    {
       try
       {        
           auto files = filesTask.get();
           if (files->Size > 0)
           {
             return GetFileDateTimeAsync(files->GetAt(0));
           }
           else
           {
              DateTime defaultDateTime = {0ll};
              return create_task_from_result<DateTime>(defaultDateTime);
           }
       }
       catch(Platform::Exception^)
       {
         DateTime defaultDateTime = {0ll};
         return create_task_from_result<DateTime>(defaultDateTime);
       }
    }, task_continuation_context::use_arbitrary()).then([repository, folderQuery, exceptionPolicy](DateTime yearDate) -> YearGroup^
    {
        return (yearDate.UniversalTime != 0) ? ref new YearGroup(yearDate, folderQuery, repository, exceptionPolicy) : nullptr; 
    }, task_continuation_context::use_arbitrary()).then(ObserveException<YearGroup^>(exceptionPolicy));
}



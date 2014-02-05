// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#pragma once

#include "..\Hilo\Repository.h"
#include "..\Hilo\PageType.h"

namespace HiloTests
{
    class StubExceptionPolicy;

    class StubRepository : public Hilo::Repository
    {
    public:
        StubRepository(std::shared_ptr<StubExceptionPolicy> exceptionPolicy);

        virtual void AddObserver(const std::function<void()> callback, Hilo::PageType pageType);
        virtual void RemoveObserver(Hilo::PageType pageType);

        virtual concurrency::task<Windows::Foundation::Collections::IVectorView<Hilo::IPhotoGroup^>^> GetMonthGroupedPhotosWithCacheAsync(std::shared_ptr<Hilo::PhotoCache> photoCache, concurrency::cancellation_token token);
        virtual concurrency::task<Hilo::IPhotoImage^> GetSinglePhotoAsync(Platform::String^ photoPath);
        virtual concurrency::task<unsigned int> GetFolderPhotoCountAsync(Windows::Storage::Search::IStorageFolderQueryOperations^ folderQuery);
        virtual concurrency::task<Windows::Foundation::Collections::IVectorView<Hilo::IPhoto^>^> GetPhotoDataForMonthGroup(Hilo::IPhotoGroup^ photoGroup, Windows::Storage::Search::IStorageFolderQueryOperations^ folderQuery, unsigned int maxNumberOfItems);
        virtual concurrency::task<bool> HasPhotosInRangeAsync(Platform::String^ dateRangeQuery, Windows::Storage::Search::IStorageFolderQueryOperations^ folderQuery);
        virtual concurrency::task<Windows::Foundation::Collections::IVectorView<Hilo::IPhoto^>^> GetPhotosForDateRangeQueryAsync(Platform::String^ dateRangeQuery);
        virtual concurrency::task<Windows::Foundation::Collections::IVectorView<Hilo::IPhoto^>^> GetPhotosForPictureHubGroupAsync(Hilo::IPhotoGroup^ photoGroup, unsigned int maxNumberOfItems);
        virtual concurrency::task<Windows::Foundation::Collections::IVectorView<Hilo::IYearGroup^>^> GetYearGroupedMonthsAsync(concurrency::cancellation_token token);
        virtual concurrency::task<Windows::Foundation::Collections::IVectorView<Windows::Storage::StorageFile^>^> GetPhotoStorageFilesAsync(Platform::String^ query, unsigned int maxNumberOfItems = 25);
        virtual void NotifyAllObservers();

        bool GetMonthGroupedPhotosWithCacheAsyncHasBeenCalled();
        bool GetSinglePhotoAsyncHasBeenCalled();
        bool GetFolderPhotoCountAsyncHasBeenCalled();
        bool GetPhotoDataForMonthGroupHasBeenCalled();
        bool GetPhotosForDateRangeQueryAsyncHasBeenCalled();
        bool GetPhotosForPictureHubGroupAsyncHasBeenCalled();
        bool GetYearGroupedMonthsAsyncHasBeenCalled();
        bool GetPhotoStorageFilesAsyncHasBeenCalled();
        bool GetAddObserverHasBeenCalled();
        bool GetRemoveObserverHasBeenCalled();
        void SetPhotoToReturn(Hilo::IPhoto^ photo);

    private:
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
        bool m_getMonthGroupedPhotosWithCacheAsyncCalled;
        bool m_getSinglePhotoAsyncCalled;
        bool m_GetFolderPhotoCountAsyncHasBeenCalled;
        bool m_getPhotoDataForMonthGroupHasBeenCalled;
        bool m_getPhotosForDateRangeQueryAsyncCalled;
        bool m_getPhotosForPictureHubgGroupAsyncCalled;
        bool m_getYearGroupedMonthsAsyncCalled;
        bool m_getPhotoStorageFilesAsyncCalled;
        bool m_addObserverCalled;
        bool m_removeObserverCalled;
        Hilo::IPhoto^ m_photoToReturn;
    };
}

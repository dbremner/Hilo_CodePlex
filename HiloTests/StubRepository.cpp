// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "StubRepository.h"
#include "StubPhoto.h"
#include "StubPhotoImage.h"
#include "..\Hilo\IPhotoGroup.h"
#include "..\Hilo\PhotoCache.h"
#include "..\Hilo\IYearGroup.h"

using namespace concurrency;
using namespace Hilo;
using namespace HiloTests;
using namespace std;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Search;

StubRepository::StubRepository(shared_ptr<StubExceptionPolicy> exceptionPolicy) : m_exceptionPolicy(exceptionPolicy)
{
}

void StubRepository::AddObserver(const std::function<void()> callback, PageType pageType)
{
    (void) pageType;
    m_addObserverCalled = true;
}

void StubRepository::RemoveObserver(PageType pageType)
{
    (void) pageType;
    m_removeObserverCalled = true;
}
void StubRepository::NotifyAllObservers()
{
}

task<IVectorView<IPhotoGroup^>^> StubRepository::GetMonthGroupedPhotosWithCacheAsync(std::shared_ptr<Hilo::PhotoCache> photoCache, cancellation_token token)
{
    (void) token;
    m_getMonthGroupedPhotosWithCacheAsyncCalled = true;
    return create_task([] 
    {
        auto temp = ref new Vector<IPhotoGroup^>();
        return temp->GetView();
    });
}

task<IPhotoImage^> StubRepository::GetSinglePhotoAsync(String^ photoPath)
{
    m_getSinglePhotoAsyncCalled = true;
    return create_task([photoPath] 
    {
        auto photo = ref new StubPhotoImage();
        photo->Path = photoPath;
        return static_cast<IPhotoImage^>(photo);
    });
}

task<unsigned int> StubRepository::GetFolderPhotoCountAsync(IStorageFolderQueryOperations^ folderQuery)
{
    (void) folderQuery;

    m_GetFolderPhotoCountAsyncHasBeenCalled = true;
    return create_task([this] 
    {
        return 42U;
    });
}
 
task<IVectorView<IPhoto^>^> StubRepository::GetPhotoDataForMonthGroup(IPhotoGroup^ photoGroup, IStorageFolderQueryOperations^ folderQuery, unsigned int maxNumberOfItems)
{
    (void) folderQuery;
    (void) maxNumberOfItems;

    m_getPhotoDataForMonthGroupHasBeenCalled = true;
    return create_task([this] 
    {
        auto temp = ref new Vector<IPhoto^>();
        if (nullptr != m_photoToReturn)
        {
            temp->Append(m_photoToReturn);
        }
        return temp->GetView();
    });
}

task<bool> StubRepository::HasPhotosInRangeAsync(Platform::String^ dateRangeQuery, Windows::Storage::Search::IStorageFolderQueryOperations^ folderQuery)
{
    return create_task([]() { return true; });
}

task<IVectorView<Hilo::IPhoto^>^> StubRepository::GetPhotosForDateRangeQueryAsync(String^ dateRangeQuery)
{
    m_getPhotosForDateRangeQueryAsyncCalled = true;
    return create_task([this] 
    {
        auto temp = ref new Vector<Hilo::IPhoto^>();
        if (nullptr != m_photoToReturn)
        {
            temp->Append(m_photoToReturn);
        }
        return temp->GetView();
    });
}

task<IVectorView<Hilo::IPhoto^>^> StubRepository::GetPhotosForPictureHubGroupAsync(Hilo::IPhotoGroup^ photoGroup, unsigned int maxNumberOfItems)
{
    (void) maxNumberOfItems;

    m_getPhotosForPictureHubgGroupAsyncCalled = true;
    return create_task([this] 
    {
        auto temp = ref new Vector<Hilo::IPhoto^>();
        if (nullptr != m_photoToReturn)
        {
            temp->Append(m_photoToReturn);
        }
        return temp->GetView();
    });
}

task<IVectorView<IYearGroup^>^> StubRepository::GetYearGroupedMonthsAsync(concurrency::cancellation_token token)
{
    (void)token;
    m_getYearGroupedMonthsAsyncCalled = true;
    return create_task([] 
    {
        auto temp = ref new Vector<IYearGroup^>();
        return temp->GetView();
    });
}

task<IVectorView<StorageFile^>^> StubRepository::GetPhotoStorageFilesAsync(Platform::String^ query, unsigned int maxNumberOfItems)
{
    (void) query;
    (void) maxNumberOfItems;

    m_getPhotoStorageFilesAsyncCalled = true;
    return create_task([]
    {
        auto temp = ref new Vector<StorageFile^>();
        return temp->GetView();
    });
}

bool StubRepository::GetMonthGroupedPhotosWithCacheAsyncHasBeenCalled()
{
    return m_getMonthGroupedPhotosWithCacheAsyncCalled;
}

bool StubRepository::GetSinglePhotoAsyncHasBeenCalled()
{
    return m_getSinglePhotoAsyncCalled;
}

bool StubRepository::GetFolderPhotoCountAsyncHasBeenCalled()
{
    return m_GetFolderPhotoCountAsyncHasBeenCalled;
}

bool StubRepository::GetPhotoDataForMonthGroupHasBeenCalled()
{
    return m_getPhotoDataForMonthGroupHasBeenCalled;
}

bool StubRepository::GetPhotosForDateRangeQueryAsyncHasBeenCalled()
{
    return m_getPhotosForDateRangeQueryAsyncCalled;
}

bool StubRepository::GetPhotosForPictureHubGroupAsyncHasBeenCalled()
{
    return m_getPhotosForPictureHubgGroupAsyncCalled;
}

bool StubRepository::GetYearGroupedMonthsAsyncHasBeenCalled()
{
    return m_getYearGroupedMonthsAsyncCalled;
}

bool StubRepository::GetPhotoStorageFilesAsyncHasBeenCalled()
{
    return m_getPhotoStorageFilesAsyncCalled;
}

void StubRepository::SetPhotoToReturn(Hilo::IPhoto^ photo)
{
    m_photoToReturn = photo;
}

bool StubRepository::GetAddObserverHasBeenCalled()
{
    return m_addObserverCalled;
}

bool StubRepository::GetRemoveObserverHasBeenCalled()
{
    return m_removeObserverCalled;
}


//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "FileRepository.h"
#include "PhotoQueryBuilder.h"
#include "Photo.h"
#include "MonthGroup.h"
#include "YearGroup.h"
#include "IQueryOperation.h"
#include "FolderQueryOperation.h"
#include "PhotoGroupData.h"
#include "LocalResourceLoader.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Search;
using namespace Windows::UI::Core;

#define MaxNumberOfThumbnailsPerMonthGroup 9

FileRepository::FileRepository(IExceptionPolicy^ exceptionPolicy) : m_exceptionPolicy(exceptionPolicy)
{
}

FileRepository::~FileRepository()
{
    RemoveObserver();
}

IAsyncOperation<IVectorView<IPhoto^>^>^ FileRepository::GetPhotosForGroupWithQueryOperationAsync(IPhotoGroup^ photoGroup, IQueryOperation^ operation)
{
    PhotoQueryBuilder query;
    PhotoQueryBuilderResult<FileInformation^> result;
    IStorageFolderQueryOperations^ queryOptions = dynamic_cast<IStorageFolderQueryOperations^>(operation->GetOperator());
    if (queryOptions == nullptr)
    {
        if (operation->MaxNumberOfItems > 0)
        {
            result = query.GetPhotosAsync(operation->Query, operation->MaxNumberOfItems);
        }
        else
        {
            result = query.GetAllPhotosAsync(operation->Query);
        }
    }
    else if (operation->MaxNumberOfItems > 0)
    {
        result = query.GetPhotosAsync(queryOptions, operation->Query);
    }
    else
    {
        result = query.GetAllPhotosAsync(queryOptions, operation->Query);
    }
    auto t = result.GetStorageItemsTask();
    RemoveObserver();
    if (m_observed)
    {
        m_queryResult = result.GetQueryResult();
        m_watchToken = m_queryResult->ContentsChanged += ref new TypedEventHandler<IStorageQueryResultBase^, Object^>(this, &FileRepository::OnFileQueryContentsChanged);
    }
    //AddObserver<FileInformation^>(result);
    IExceptionPolicy^ policy = m_exceptionPolicy;
    return create_async([t, photoGroup, policy]
    {        
        return t.then([photoGroup, policy](IVectorView<FileInformation^>^ files) 
        {
            auto photos = ref new Vector<IPhoto^>();
            for (auto item : files)
            {
                auto photo = ref new Photo(item, photoGroup, policy);
                photos->Append(photo);
            }
            return photos->GetView();
        }, task_continuation_context::use_arbitrary());
    });
}

IAsyncOperation<PhotoGroupData^>^ FileRepository::GetPhotoGroupDataForGroupWithQueryOperationAsync(IPhotoGroup^ photoGroup, IQueryOperation^ operation)
{
    PhotoQueryBuilder query;
    PhotoQueryBuilderResult<FileInformation^> result;
    IStorageFolderQueryOperations^ queryOptions = dynamic_cast<IStorageFolderQueryOperations^>(operation->GetOperator());
    result = query.GetAllPhotosAsync(queryOptions, operation->Query);
    auto t = result.GetStorageItemsTask();
    RemoveObserver();
    if (m_observed)
    {
        m_queryResult = result.GetQueryResult();
        m_watchToken = m_queryResult->ContentsChanged += ref new TypedEventHandler<IStorageQueryResultBase^, Object^>(this, &FileRepository::OnFileQueryContentsChanged);
    }
    //AddObserver<FileInformation^>(result);
    IExceptionPolicy^ policy = m_exceptionPolicy;
    unsigned int maxItems = operation->MaxNumberOfItems;
    return create_async([t, photoGroup, policy, maxItems]
    {        
        return t.then([photoGroup, policy, maxItems](IVectorView<FileInformation^>^ files) 
        {
            auto photos = ref new Vector<IPhoto^>();
            unsigned int count = 0;
            for (auto item : files)
            {
                if (count == maxItems) 
                {
                    break;
                }
                auto photo = ref new Photo(item, photoGroup, policy);
                photos->Append(photo);
                count++;
            }
            return ref new PhotoGroupData(photos->GetView(), files->Size);
        }, task_continuation_context::use_arbitrary());
    });
}

IAsyncOperation<IVectorView<IPhotoGroup^>^>^ FileRepository::GetMonthGroupedPhotosWithCacheAsync(IPhotoCache^ photoCache)
{
    IExceptionPolicy^ policy = m_exceptionPolicy;
    IRepository^ repository = this;
    return create_async([photoCache, repository, policy]
    { 
        PhotoQueryBuilder query;
        auto foldersTask = query.GetVirtualPhotoFoldersByMonth();
        return foldersTask.then([photoCache, repository, policy](IVectorView<FolderInformation^>^ folders) 
        {
            auto temp = ref new Vector<IPhotoGroup^>();
            for (auto folder : folders)
            {
                auto photoGroup = ref new MonthGroup(folder->Name, photoCache, repository, ref new FolderQueryOperation(folder, MaxNumberOfThumbnailsPerMonthGroup), policy);
                temp->Append(photoGroup);
            }
            return temp->GetView();
        }, task_continuation_context::use_arbitrary());
    });
}

IAsyncOperation<IVectorView<IYearGroup^>^>^ FileRepository::GetYearGroupedMonthsAsync()
{
    IExceptionPolicy^ policy = m_exceptionPolicy;
    IRepository^ repository = this;
    return create_async([policy, repository]
    { 
        PhotoQueryBuilder query;
        auto foldersTask = query.GetVirtualPhotoFoldersByYear();
        return foldersTask.then([policy, repository](IVectorView<FolderInformation^>^ folders) 
        {
            auto yearGroups = ref new Vector<IYearGroup^>();
            for (auto folder : folders)
            {
                auto operation = ref new FolderQueryOperation(folder);
                auto yearGroup = ref new YearGroup(folder->Name, repository, operation, policy);

                yearGroups->Append(yearGroup);
            }
            return yearGroups->GetView();
        }, task_continuation_context::use_arbitrary());
    });
}

IAsyncOperation<unsigned int>^ FileRepository::GetPhotoCountForQueryOperationAsync(IQueryOperation^ operation)
{

    return create_async([operation]
    {
        IStorageFolderQueryOperations^ queryOptions = dynamic_cast<IStorageFolderQueryOperations^>(operation->GetOperator());
        assert(nullptr != queryOptions);
        auto dateRangeQuery = operation->Query;
        assert(nullptr != dateRangeQuery);
        PhotoQueryBuilder query;
        auto result = query.GetPhotosAsync(queryOptions, dateRangeQuery);
        return result.GetStorageItemsTask().then([](IVectorView<FileInformation^>^ files)
        {
            return files->Size;
        }, task_continuation_context::use_arbitrary());
    });
}

IAsyncOperation<IPhoto^>^ FileRepository::GetPhotoForGroupWithQueryOperationAsync(IPhotoGroup^ photoGroup, IQueryOperation^ operation)
{
    IExceptionPolicy^ policy = m_exceptionPolicy;
    return create_async([photoGroup, operation, policy]
    {
        PhotoQueryBuilder query;
        auto result = query.GetPhotosAsync(operation->Query, 1);
        return result.GetStorageItemsTask().then([photoGroup, policy](IVectorView<FileInformation^>^ files) 
        {
            IPhoto^ photo = nullptr;;
            auto size = files->Size;
            if (size > 0)
            {
                photo = ref new Photo(files->GetAt(0), photoGroup, policy);
            }
            return photo;
        }, task_continuation_context::use_arbitrary());
    });
}

void FileRepository::RemoveObserver()
{
    if (m_observed && nullptr != m_queryResult && m_watchToken.Value != 0)
    {
        m_queryResult->ContentsChanged -= m_watchToken;
    }
}

EventRegistrationToken FileRepository::DataChanged::add(DataChangedEventHandler^ e)
{
    m_observed = true;
    return m_dataChangedEvent += e;
}

void FileRepository::DataChanged::remove(EventRegistrationToken t)
{
    m_observed = false;
    m_queryResult->ContentsChanged -= m_watchToken;
    m_queryResult = nullptr;
    m_dataChangedEvent -= t;
}

void FileRepository::OnFileQueryContentsChanged(IStorageQueryResultBase^ sender, Object^ e)
{
    CoreWindow^ wnd = CoreApplication::MainView->CoreWindow;
    CoreDispatcher^ dispatcher = wnd->Dispatcher;
    dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this] () 
    {
        m_dataChangedEvent();
    }));

}
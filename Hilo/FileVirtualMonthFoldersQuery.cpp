//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "FileVirtualMonthFoldersQuery.h"
#include "MonthGroup.h"
#include "ExceptionPolicy.h"
#include "PhotoQueryBuilder.h"
#include "FileMonthGroupQuery.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace std;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::BulkAccess;

FileVirtualMonthFoldersQuery::FileVirtualMonthFoldersQuery(shared_ptr<ExceptionPolicy> exceptionPolicy) : m_exceptionPolicy(exceptionPolicy)
{
}

task<IVectorView<IPhotoGroup^>^> FileVirtualMonthFoldersQuery::GetMonthGroupedPhotosWithCacheAsync(shared_ptr<PhotoCache> photoCache, cancellation_token token)
{
    PhotoQueryBuilder query;
    auto result = query.GetVirtualPhotoFoldersByMonth(token);
    auto t = result.GetStorageItemsTask();
    RemoveObserver();
    if (m_callback)
    {
        m_pictureHubGroupQueryChange = ref new QueryChange(result.GetQueryResult(), m_callback);
    }
    shared_ptr<ExceptionPolicy> policy = m_exceptionPolicy;
    return t.then([photoCache, policy](IVectorView<FolderInformation^>^ folders) 
    {
        assert(IsBackgroundThread());

        auto temp = ref new Vector<IPhotoGroup^>();
        for (auto folder : folders)
        {
            auto photoGroup = ref new MonthGroup(folder->Name, photoCache, make_shared<FileMonthGroupQuery>(folder, policy), policy);
            temp->Append(photoGroup);
        }
        return temp->GetView();
    }, task_continuation_context::use_arbitrary());
}

void FileVirtualMonthFoldersQuery::AddObserver(const function<void()> callback)
{
    m_callback = callback;
}

void FileVirtualMonthFoldersQuery::RemoveObserver()
{
    m_pictureHubGroupQueryChange = nullptr;
}
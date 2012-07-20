//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "FileVirtualYearFoldersQuery.h"
#include "PhotoQueryBuilder.h"
#include "YearGroup.h"
#include "ExceptionPolicy.h"
#include "FileMonthBlockQuery.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform::Collections;
using namespace std;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::BulkAccess;

FileVirtualYearFoldersQuery::FileVirtualYearFoldersQuery(shared_ptr<ExceptionPolicy> exceptionPolicy) : m_exceptionPolicy(exceptionPolicy)
{
}

task<IVectorView<IYearGroup^>^> FileVirtualYearFoldersQuery::GetYearGroupedMonthsAsync(cancellation_token token)
{
    shared_ptr<ExceptionPolicy> policy = m_exceptionPolicy;
    PhotoQueryBuilder query;
    auto foldersTask = query.GetVirtualPhotoFoldersByYear(token);
    return foldersTask.then([policy](IVectorView<FolderInformation^>^ folders) 
    {
        auto yearGroups = ref new Vector<IYearGroup^>();
        for (auto folder : folders)
        {
            auto yearGroup = ref new YearGroup(folder->Name, make_shared<FileMonthBlockQuery>(folder, policy), policy);

            yearGroups->Append(yearGroup);
        }
        return yearGroups->GetView();
    }, task_continuation_context::use_arbitrary());
}
//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "FileMonthBlockQuery.h"
#include "PhotoQueryBuilder.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace std;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Search;

FileMonthBlockQuery::FileMonthBlockQuery(IStorageFolderQueryOperations^ folderQuery, shared_ptr<ExceptionPolicy> exceptionPolicy) : m_folderQuery(folderQuery), m_exceptionPolicy(exceptionPolicy)
{
}

task<unsigned int> FileMonthBlockQuery::GetPhotoCountWithDateRangeQueryAsync(String^ dateRangeQuery, cancellation_token token)
{
    PhotoQueryBuilder builder;
    auto result = builder.GetPhotosAsync(m_folderQuery, dateRangeQuery, token, 1);
    auto t = result.GetStorageItemsTask();
    return t.then([](IVectorView<FileInformation^>^ files)
    {
        return files->Size;
    }, task_continuation_context::use_arbitrary());
}
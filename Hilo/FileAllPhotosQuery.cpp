//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "FileAllPhotosQuery.h"
#include "QueryChange.h"
#include "ExceptionPolicy.h"
#include "PhotoQueryBuilder.h"
#include "NullPhotoGroup.h"
#include "Photo.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace std;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::BulkAccess;

FileAllPhotosQuery::FileAllPhotosQuery(shared_ptr<ExceptionPolicy> exceptionPolicy) : m_exceptionPolicy(exceptionPolicy)
{
}

task<IVectorView<IPhoto^>^> FileAllPhotosQuery::GetPhotosForDateRangeQueryAsync(Platform::String^ dateRangeQuery, cancellation_token token)
{
    PhotoQueryBuilder query;
    PhotoQueryBuilderResult<FileInformation^> result;
    
    result = query.GetAllPhotosAsync(dateRangeQuery, token);
    RemoveObserver();
    if (m_callback)
    {
        m_photosQueryChange = ref new QueryChange(result.GetQueryResult(), m_callback);
    }
    auto t = result.GetStorageItemsTask();
    shared_ptr<ExceptionPolicy> policy = m_exceptionPolicy;
    return t.then([policy](IVectorView<FileInformation^>^ files) 
    {
        auto photos = ref new Vector<IPhoto^>();
        for (auto file : files)
        {
            auto photo = ref new Photo(file, ref new NullPhotoGroup(), policy);
            photos->Append(photo);
        }
        return photos->GetView();
    }, task_continuation_context::use_arbitrary());
}

void FileAllPhotosQuery::AddObserver(const std::function<void()> callback)
{
    m_callback = callback;
}

void FileAllPhotosQuery::RemoveObserver()
{
    m_photosQueryChange = nullptr;
}
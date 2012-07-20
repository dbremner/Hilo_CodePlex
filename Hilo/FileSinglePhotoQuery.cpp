//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "FileSinglePhotoQuery.h"
#include "PhotoQueryBuilder.h"
#include "NullPhotoGroup.h"
#include "Photo.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace std;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::BulkAccess;

FileSinglePhotoQuery::FileSinglePhotoQuery(shared_ptr<ExceptionPolicy> exceptionPolicy) : m_exceptionPolicy(exceptionPolicy)
{
}

task<IPhoto^> FileSinglePhotoQuery::GetPhotoAsync(String^ photoPath, cancellation_token token)
{
    PhotoQueryBuilder query;
    auto result = query.GetPhotoAsync(photoPath, token);
    shared_ptr<ExceptionPolicy> policy = m_exceptionPolicy;
    return result.GetStorageItemsTask().then([policy](IVectorView<FileInformation^>^ files) 
    {
        IPhoto^ photo = nullptr;
        auto size = files->Size;
        if (size > 0)
        {
            photo = ref new Photo(files->GetAt(0), ref new NullPhotoGroup(), policy);
        }
        return photo;
    }, task_continuation_context::use_arbitrary());
}
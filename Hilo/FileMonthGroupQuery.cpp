//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "FileMonthGroupQuery.h"
#include "PhotoGroupData.h"
#include "Photo.h"
#include "PhotoQueryBuilder.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform::Collections;
using namespace std;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Search;

FileMonthGroupQuery::FileMonthGroupQuery(IStorageFolderQueryOperations^ folderQuery, shared_ptr<ExceptionPolicy> exceptionPolicy) : m_folderQuery(folderQuery), m_exceptionPolicy(exceptionPolicy)
{
}

task<PhotoGroupData> FileMonthGroupQuery::GetPhotoDataForMonthGroup(IPhotoGroup^ photoGroup, unsigned int maxNumberOfItems, cancellation_token token)
{
    PhotoQueryBuilder query;
    PhotoQueryBuilderResult<FileInformation^> result;
    result = query.GetAllPhotosAsync(m_folderQuery, "", token);
    auto t = result.GetStorageItemsTask();
    shared_ptr<ExceptionPolicy> policy = m_exceptionPolicy;
    return t.then([photoGroup, policy, maxNumberOfItems](IVectorView<FileInformation^>^ files) 
    {
        auto photos = ref new Vector<IPhoto^>();
        unsigned int count = 0;
        for (auto item : files)
        {
            if (count == maxNumberOfItems) 
            {
                break;
            }
            auto photo = ref new Photo(item, photoGroup, policy);
            photos->Append(photo);
            count++;
        }
        return PhotoGroupData(photos->GetView(), files->Size);
    }, task_continuation_context::use_arbitrary());
}

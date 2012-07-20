//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "FilePictureHubGroupQuery.h"
#include "PhotoQueryBuilder.h"
#include "QueryChange.h"
#include "IPhotoGroup.h"
#include "Photo.h"
#include "ExceptionPolicy.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace std;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::BulkAccess;

FilePictureHubGroupQuery::FilePictureHubGroupQuery(shared_ptr<ExceptionPolicy> exceptionPolicy) : m_exceptionPolicy(exceptionPolicy)
{
}

task<IVectorView<IPhoto^>^> FilePictureHubGroupQuery::GetPhotosForPictureHubGroupAsync(IPhotoGroup^ photoGroup, unsigned int maxNumberOfItems, cancellation_token token)
{
    PhotoQueryBuilder query;
    PhotoQueryBuilderResult<FileInformation^> result;
    
    result = query.GetPhotosAsync("", token, maxNumberOfItems);
    RemoveObserver();
    if (m_callback)
    {
        m_pictureHubGroupQueryChange = ref new QueryChange(result.GetQueryResult(), m_callback);
    }
    auto t = result.GetStorageItemsTask();
    shared_ptr<ExceptionPolicy> policy = m_exceptionPolicy;
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
}

void FilePictureHubGroupQuery::AddObserver(const std::function<void()> callback)
{
    m_callback = callback;
}

void FilePictureHubGroupQuery::RemoveObserver()
{
    m_pictureHubGroupQueryChange = nullptr;
}
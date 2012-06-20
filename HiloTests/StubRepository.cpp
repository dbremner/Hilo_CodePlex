//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "StubRepository.h"
#include "..\Hilo\IPhotoGroup.h"
#include "..\Hilo\IPhoto.h"
#include "..\Hilo\IPhotoCache.h"
#include "..\Hilo\IQueryOperation.h"
#include "..\Hilo\IYearGroup.h"
#include "..\Hilo\PhotoGroupData.h"
#include "StubPhoto.h"

using namespace concurrency;
using namespace Hilo;
using namespace HiloTests;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

StubRepository::StubRepository()
{
    GetMonthGroupedPhotosWithCacheAsyncCalled = false;
    GetPhotoGroupDataForGroupWithQueryOperationAsyncCalled = false;
    GetYearGroupedMonthsAsyncCalled = false;
    GetPhotoCountForQueryOperationAsyncCalled = false;
    GetPhotoForGroupWithQueryOperationAsyncCalled = false;

    DataChangedEventObserved = false;
}

IAsyncOperation<IVectorView<IPhoto^>^>^ StubRepository::GetPhotosForGroupWithQueryOperationAsync(IPhotoGroup^ photoGroup, IQueryOperation^ operation)
{
    GetPhotosForGroupWithQueryOperationAsyncCalled = true;
    return create_async([this]
    { 
        return create_task([this] 
        {
            auto temp = ref new Vector<IPhoto^>();
            if (nullptr != PhotoToReturn)
            {
                temp->Append(PhotoToReturn);
            }
            return temp->GetView();
        });
    });
}

IAsyncOperation<Hilo::PhotoGroupData^>^ StubRepository::GetPhotoGroupDataForGroupWithQueryOperationAsync(IPhotoGroup^ photoGroup, IQueryOperation^ operation)
{
    GetPhotoGroupDataForGroupWithQueryOperationAsyncCalled = true;
    return create_async([this]
    { 
        return create_task([this] 
        {
            auto temp = ref new Vector<IPhoto^>();
            if (nullptr != PhotoToReturn)
            {
                temp->Append(PhotoToReturn);
            }
            return ref new PhotoGroupData(temp->GetView(), temp->Size);
        });
    });
}

IAsyncOperation<IVectorView<IPhotoGroup^>^>^ StubRepository::GetMonthGroupedPhotosWithCacheAsync(IPhotoCache^ photoCache)
{
    GetMonthGroupedPhotosWithCacheAsyncCalled = true;

    return create_async([]
    { 
        return create_task([] 
        {
            auto temp = ref new Vector<IPhotoGroup^>();
            return temp->GetView();
        });
    });
}

IAsyncOperation<IVectorView<IYearGroup^>^>^ StubRepository::GetYearGroupedMonthsAsync()
{
    GetYearGroupedMonthsAsyncCalled = true;

    return create_async([]
    { 
        return create_task([] 
        {
            auto temp = ref new Vector<IYearGroup^>();
            return temp->GetView();
        });
    });
}

IAsyncOperation<unsigned int>^ StubRepository::GetPhotoCountForQueryOperationAsync(IQueryOperation^ operation)
{
    GetPhotoCountForQueryOperationAsyncCalled = true;

    return create_async([]
    { 
        return create_task([] 
        {
            return 42U;
        });
    });
}

IAsyncOperation<IPhoto^>^ StubRepository::GetPhotoForGroupWithQueryOperationAsync(IPhotoGroup^ photoGroup, IQueryOperation^ operation)
{
    GetPhotoForGroupWithQueryOperationAsyncCalled = true;

    return create_async([]
    { 
        return create_task([] 
        {
            IPhoto^ photo = ref new StubPhoto();
            return photo;
        });
    });
}

EventRegistrationToken StubRepository::DataChanged::add(DataChangedEventHandler^ e)
{
    DataChangedEventObserved = true;
    m_observed = true;
    return m_dataChangedEvent += e;
}

void StubRepository::DataChanged::remove(EventRegistrationToken t)
{
    DataChangedEventObserved = false;
    m_observed = false;
    m_dataChangedEvent -= t;
}
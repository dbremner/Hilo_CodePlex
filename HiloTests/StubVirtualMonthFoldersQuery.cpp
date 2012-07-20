//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "StubVirtualMonthFoldersQuery.h"
#include "..\Hilo\IPhotoGroup.h"
#include "..\Hilo\PhotoCache.h"

using namespace concurrency;
using namespace Hilo;
using namespace HiloTests;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;

task<IVectorView<IPhotoGroup^>^> StubVirtualMonthFoldersQuery::GetMonthGroupedPhotosWithCacheAsync(std::shared_ptr<Hilo::PhotoCache> photoCache, cancellation_token token)
{
    return create_task([] 
    {
        auto temp = ref new Vector<IPhotoGroup^>();
        return temp->GetView();
    }, token);
}

void StubVirtualMonthFoldersQuery::AddObserver(const std::function<void()> callback)
{
    m_observed = true;
}

void StubVirtualMonthFoldersQuery::RemoveObserver()
{
    m_observed = false;
}

bool StubVirtualMonthFoldersQuery::GetHasBeenCalled()
{
    return m_called;
}

bool StubVirtualMonthFoldersQuery::GetObserved()
{
    return m_observed;
}

//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"

#include "StubAllPhotosQuery.h"
#include "..\Hilo\IPhoto.h"

using namespace concurrency;
using namespace HiloTests;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;

task<IVectorView<Hilo::IPhoto^>^> StubAllPhotosQuery::GetPhotosForDateRangeQueryAsync(String^ dateRangeQuery, cancellation_token token)
{
    return create_task([this] 
    {
        auto temp = ref new Vector<Hilo::IPhoto^>();
        if (nullptr != m_photoToReturn)
        {
            temp->Append(m_photoToReturn);
        }
        return temp->GetView();
    }, token);
}

void StubAllPhotosQuery::AddObserver(const std::function<void()> callback)
{
    m_observed = true;
}

void StubAllPhotosQuery::RemoveObserver()
{
    m_observed = false;
}

bool StubAllPhotosQuery::GetHasBeenCalled()
{
    return m_called;
}

bool StubAllPhotosQuery::GetObserved()
{
    return m_observed;
}

void StubAllPhotosQuery::SetPhotoToReturn(Hilo::IPhoto^ photo)
{
    m_photoToReturn = photo;
}
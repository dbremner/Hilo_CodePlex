//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"

#include "StubPictureHubGroupQuery.h"
#include "..\Hilo\IPhoto.h"

using namespace concurrency;
using namespace HiloTests;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;

task<IVectorView<Hilo::IPhoto^>^> StubPictureHubGroupQuery::GetPhotosForPictureHubGroupAsync(Hilo::IPhotoGroup^ photoGroup, unsigned int maxNumberOfItems, cancellation_token token)
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

void StubPictureHubGroupQuery::AddObserver(const std::function<void()> callback)
{
    m_observed = true;
}

void StubPictureHubGroupQuery::RemoveObserver()
{
    m_observed = false;
}

bool StubPictureHubGroupQuery::GetHasBeenCalled()
{
    return m_called;
}

bool StubPictureHubGroupQuery::GetObserved()
{
    return m_observed;
}

void StubPictureHubGroupQuery::SetPhotoToReturn(Hilo::IPhoto^ photo)
{
    m_photoToReturn = photo;
}
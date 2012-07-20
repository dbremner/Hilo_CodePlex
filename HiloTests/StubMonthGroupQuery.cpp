//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "StubMonthGroupQuery.h"
#include "..\Hilo\IPhoto.h"

using namespace concurrency;
using namespace Hilo;
using namespace HiloTests;
using namespace Platform::Collections;

task<PhotoGroupData> StubMonthGroupQuery::GetPhotoDataForMonthGroup(IPhotoGroup^ photoGroup, unsigned int maxNumberOfItems, cancellation_token token)
{
    m_called = true;
    return create_task([this] 
    {
        auto temp = ref new Vector<IPhoto^>();
        if (nullptr != m_photoToReturn)
        {
            temp->Append(m_photoToReturn);
        }
        return PhotoGroupData(temp->GetView(), temp->Size);
    }, token);
}

bool StubMonthGroupQuery::GetHasBeenCalled()
{
    return m_called;
}

void StubMonthGroupQuery::SetPhotoToReturn(Hilo::IPhoto^ photo)
{
    m_photoToReturn = photo;
}
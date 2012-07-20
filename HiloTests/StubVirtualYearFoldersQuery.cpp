//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "StubVirtualYearFoldersQuery.h"
#include "..\Hilo\IYearGroup.h"

using namespace concurrency;
using namespace Hilo;
using namespace HiloTests;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;

task<IVectorView<IYearGroup^>^> StubVirtualYearFoldersQuery::GetYearGroupedMonthsAsync(cancellation_token token)
{
    return create_task([] 
    {
        auto temp = ref new Vector<IYearGroup^>();
        return temp->GetView();
    }, token);
}

bool StubVirtualYearFoldersQuery::GetHasBeenCalled()
{
    return m_called;
}
//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "StubMonthBlockQuery.h"
#include "StubPhoto.h"

using namespace concurrency;
using namespace Hilo;
using namespace HiloTests;
using namespace Platform;

task<unsigned int> StubMonthBlockQuery::GetPhotoCountWithDateRangeQueryAsync(String^ dateRangeQuery, cancellation_token token)
{
    m_dateRangeQuery = dateRangeQuery;
    m_called = true;
    return create_task([this] 
    {
        return 42U;
    }, token);
}

bool StubMonthBlockQuery::GetHasBeenCalled()
{
    return m_called;
}


String^ StubMonthBlockQuery::GetDateRangeQuery()
{
    return m_dateRangeQuery;
}
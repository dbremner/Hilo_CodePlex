//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "..\Hilo\MonthBlockQuery.h"

namespace HiloTests
{
    class StubMonthBlockQuery : public Hilo::MonthBlockQuery
    {
    public:
        virtual concurrency::task<unsigned int> GetPhotoCountWithDateRangeQueryAsync(Platform::String^ dateRangeQuery, concurrency::cancellation_token token);

        bool GetHasBeenCalled();
        Platform::String^ GetDateRangeQuery();
    private:
        bool m_called;
        Platform::String^ m_dateRangeQuery;
    };
}

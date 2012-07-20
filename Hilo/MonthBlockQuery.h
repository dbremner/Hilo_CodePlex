//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

namespace Hilo
{
    class MonthBlockQuery
    {
    public:
        virtual concurrency::task<unsigned int> GetPhotoCountWithDateRangeQueryAsync(Platform::String^ dateRangeQuery, concurrency::cancellation_token token) = 0;
    };
}
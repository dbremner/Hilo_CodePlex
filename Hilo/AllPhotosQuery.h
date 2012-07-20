//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "ObservableQuery.h"

namespace Hilo
{
    interface class IPhoto;

    class AllPhotosQuery : public ObservableQuery
    {
    public:
        virtual concurrency::task<Windows::Foundation::Collections::IVectorView<IPhoto^>^> GetPhotosForDateRangeQueryAsync(Platform::String^ dateRangeQuery, concurrency::cancellation_token token) = 0;
    };
}
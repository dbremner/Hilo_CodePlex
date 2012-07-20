//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "AllPhotosQuery.h"

namespace Hilo
{
    ref class QueryChange;
    class ExceptionPolicy;

    class FileAllPhotosQuery : public AllPhotosQuery
    {
    public:
        FileAllPhotosQuery(std::shared_ptr<ExceptionPolicy> exceptionPolicy);

        virtual concurrency::task<Windows::Foundation::Collections::IVectorView<IPhoto^>^> GetPhotosForDateRangeQueryAsync(Platform::String^ dateRangeQuery, concurrency::cancellation_token token);
        virtual void AddObserver(const std::function<void() > callback);
        virtual void RemoveObserver();

    private:
        QueryChange^ m_photosQueryChange;
        std::function<void()> m_callback;
        std::shared_ptr<ExceptionPolicy> m_exceptionPolicy;
    };
}
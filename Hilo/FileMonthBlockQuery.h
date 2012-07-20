//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "MonthBlockQuery.h"

namespace Hilo
{
    class ExceptionPolicy;

    class FileMonthBlockQuery : public MonthBlockQuery
    {
    public:
        FileMonthBlockQuery(Windows::Storage::Search::IStorageFolderQueryOperations^ folderQuery, std::shared_ptr<ExceptionPolicy> exceptionPolicy);
        virtual concurrency::task<unsigned int> GetPhotoCountWithDateRangeQueryAsync(Platform::String^ dateRangeQuery, concurrency::cancellation_token token);

    private:
        Windows::Storage::Search::IStorageFolderQueryOperations^ m_folderQuery;
        std::shared_ptr<ExceptionPolicy> m_exceptionPolicy;
    };
}

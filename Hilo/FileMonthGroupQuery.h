//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "MonthGroupQuery.h"

namespace Hilo
{
    class ExceptionPolicy;

    class FileMonthGroupQuery : public MonthGroupQuery
    {
    public:
        FileMonthGroupQuery(Windows::Storage::Search::IStorageFolderQueryOperations^ folderQuery, std::shared_ptr<ExceptionPolicy> exceptionPolicy);
        virtual concurrency::task<PhotoGroupData> GetPhotoDataForMonthGroup(IPhotoGroup^ photoGroup, unsigned int maxNumberOfItems, concurrency::cancellation_token token);

    private:
        Windows::Storage::Search::IStorageFolderQueryOperations^ m_folderQuery;
        std::shared_ptr<ExceptionPolicy> m_exceptionPolicy;
    };
}


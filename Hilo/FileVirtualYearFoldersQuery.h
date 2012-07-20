//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "VirtualYearFoldersQuery.h"

namespace Hilo
{
    class ExceptionPolicy;
    
    class FileVirtualYearFoldersQuery : public VirtualYearFoldersQuery
    {
    public:
        FileVirtualYearFoldersQuery(std::shared_ptr<ExceptionPolicy> exceptionPolicy);

        virtual concurrency::task<Windows::Foundation::Collections::IVectorView<IYearGroup^>^> GetYearGroupedMonthsAsync(concurrency::cancellation_token token);

    private:
        std::shared_ptr<ExceptionPolicy> m_exceptionPolicy;
    };
}
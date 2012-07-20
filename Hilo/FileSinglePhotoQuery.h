//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "SinglePhotoQuery.h"

namespace Hilo
{
    class ExceptionPolicy;

    class FileSinglePhotoQuery : public SinglePhotoQuery
    {
    public:
        FileSinglePhotoQuery(std::shared_ptr<ExceptionPolicy> exceptionPolicy);

        virtual concurrency::task<IPhoto^> GetPhotoAsync(Platform::String^ photoPath, concurrency::cancellation_token token);

    private:
        std::shared_ptr<ExceptionPolicy> m_exceptionPolicy;
    };
}


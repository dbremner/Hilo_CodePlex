//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "..\Hilo\SinglePhotoQuery.h"

namespace HiloTests
{
    class StubSinglePhotoQuery : public Hilo::SinglePhotoQuery
    {
    public:
        virtual concurrency::task<Hilo::IPhoto^> GetPhotoAsync(Platform::String^ photoPath, concurrency::cancellation_token token);

        bool GetHasBeenCalled();
    private:
        bool m_called;
    };
}

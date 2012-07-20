//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "..\Hilo\VirtualYearFoldersQuery.h"

namespace HiloTests
{
    class StubVirtualYearFoldersQuery : public Hilo::VirtualYearFoldersQuery
    {
    public:
        virtual concurrency::task<Windows::Foundation::Collections::IVectorView<Hilo::IYearGroup^>^> GetYearGroupedMonthsAsync(concurrency::cancellation_token token);

        bool GetHasBeenCalled();
    private:
        bool m_called;
    };
}

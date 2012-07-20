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
    interface class IYearGroup;

    class VirtualYearFoldersQuery
    {
    public:
        virtual concurrency::task<Windows::Foundation::Collections::IVectorView<IYearGroup^>^> GetYearGroupedMonthsAsync(concurrency::cancellation_token token) = 0;
    };
}
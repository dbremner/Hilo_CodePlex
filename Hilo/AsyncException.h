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
    class ExceptionPolicy;

    class AsyncException
    {
    public:
        static Windows::Foundation::IAsyncAction^ ObserveWithPolicy(std::shared_ptr<ExceptionPolicy> policy, Windows::Foundation::IAsyncAction^ action);
    };
}
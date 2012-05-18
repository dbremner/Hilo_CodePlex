//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "AsyncException.h"
#include "TaskExceptionsExtensions.h"

using namespace Hilo;
using namespace Windows::Foundation;

IAsyncAction^ AsyncException::ObserveWithPolicy(IExceptionPolicy^ policy, IAsyncAction^ antecedent)
{
     return concurrency::create_async([antecedent, policy]
            {
                return concurrency::task<void>([antecedent]{ return antecedent; })
                        .then(ObserveException<void>(policy));
            });
}



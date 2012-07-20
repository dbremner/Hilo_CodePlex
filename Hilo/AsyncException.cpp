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

using namespace concurrency;
using namespace Hilo;
using namespace std;
using namespace Windows::Foundation;

IAsyncAction^ AsyncException::ObserveWithPolicy(shared_ptr<ExceptionPolicy> policy, IAsyncAction^ antecedent)
{
    return create_async([antecedent, policy]
    {
        return create_task([antecedent]{ return antecedent; })
            .then(ObserveException<void>(policy));
    });
}

//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once
#include <ppltasks.h>

namespace Hilo 
{
    // Creates a task that completes with the provided result.
    template <typename Result>
    concurrency::task<Result> task_from_result(Result result)
    {
        return concurrency::task<Result>([result]() -> Result { return result; });
    }

}
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
    // Creates a task that completes with the provided result.
    template <typename Result>
    concurrency::task<Result> task_from_result(Result result)
    {
        return concurrency::create_task([result]() -> Result { return result; });
    }
}

#ifndef NDEBUG

#ifdef  __cplusplus
extern "C" {
#endif

    // Called at startup to remember the main thread id for debug assertion checking
    void RecordMainThread(void);

    // Called in debug mode only for thread context assertion checks
    bool IsMainThread(void);

    // Called in debug mode only for thread context assertion checks
    bool IsBackgroundThread(void);

#ifdef  __cplusplus
}
#endif

#endif /* not NDEBUG */
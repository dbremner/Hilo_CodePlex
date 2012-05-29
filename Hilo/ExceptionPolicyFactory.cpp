//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ExceptionPolicyFactory.h"
#include "DebugLoggingExceptionPolicy.h"
#include "windows.h"

using namespace Hilo;

IExceptionPolicy^ ExceptionPolicyFactory::GetCurrentPolicy()
{
   
    // todo:  Fix in RP.  Need to wait until RP to make sure
    //        these marshall across boundaries correctly.  For
    //          now we return a new logging policy.

    return ref new DebugLoggingExceptionPolicy();
}

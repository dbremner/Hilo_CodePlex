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

using namespace Hilo;
using namespace std;

shared_ptr<ExceptionPolicy> ExceptionPolicyFactory::GetCurrentPolicy()
{
    return std::make_shared<DebugLoggingExceptionPolicy>();
}

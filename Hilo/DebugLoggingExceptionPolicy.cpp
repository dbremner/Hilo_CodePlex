//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "DebugLoggingExceptionPolicy.h"

using namespace Hilo;
using namespace Windows::Foundation;
using namespace Platform;


void DebugLoggingExceptionPolicy::HandleException(Exception^ exception)
{
    assert(exception != nullptr);
    OutputDebugString(exception->Message->Data());
}
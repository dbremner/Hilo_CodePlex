//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "StubExceptionPolicy.h"

using namespace HiloTests;
using namespace Platform;

void StubExceptionPolicy::HandleException(Exception^ exception)
{
    m_exception = exception;
}

Exception^ StubExceptionPolicy::GetSuppliedException()
{
    return m_exception;
}

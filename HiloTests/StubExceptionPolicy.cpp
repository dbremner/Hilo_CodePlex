// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
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

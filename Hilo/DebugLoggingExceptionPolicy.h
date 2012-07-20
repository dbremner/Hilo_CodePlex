//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once
#include "ExceptionPolicy.h"

namespace Hilo 
{
    class DebugLoggingExceptionPolicy: public ExceptionPolicy
    {
    public:
        virtual void HandleException(Platform::Exception^ exception);
    };
}
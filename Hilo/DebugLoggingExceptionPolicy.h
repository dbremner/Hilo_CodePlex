//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once
#include "IExceptionPolicy.h"

namespace Hilo 
{
    public ref class DebugLoggingExceptionPolicy sealed : public IExceptionPolicy
    {
    public:
        virtual void HandleException(Platform::Exception^ exception);
    };

}
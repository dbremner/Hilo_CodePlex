//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once
#include "..\Hilo\ExceptionPolicy.h"

namespace HiloTests 
{
    class StubExceptionPolicy : public Hilo::ExceptionPolicy
    {
    public:
        Platform::Exception^ GetSuppliedException();
        virtual void HandleException(Platform::Exception^ exception);

    private:
        Platform::Exception^ m_exception;
    };
}
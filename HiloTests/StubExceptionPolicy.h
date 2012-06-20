//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once
#include "..\Hilo\IExceptionPolicy.h"

namespace HiloTests 
{
    ref class StubExceptionPolicy sealed : public Hilo::IExceptionPolicy
    {
    public:
        property Platform::Exception^ SuppliedException;
        virtual void HandleException(Platform::Exception^ exception);
    };
}
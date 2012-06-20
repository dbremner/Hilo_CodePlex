//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once
#include "..\Hilo\IResourceLoader.h"

namespace HiloTests
{
    ref class StubResourceLoader sealed : public Hilo::IResourceLoader
    {
    public:
        property Platform::String^ StringToReturn;

        virtual Platform::String^ GetString(Platform::String^ value);
    };
}
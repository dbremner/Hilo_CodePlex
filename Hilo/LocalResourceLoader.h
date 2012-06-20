//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "IResourceLoader.h"

namespace Hilo
{
    public ref class LocalResourceLoader sealed : public IResourceLoader
    {
    public:
        virtual Platform::String^ GetString(Platform::String^ value);
    };
}
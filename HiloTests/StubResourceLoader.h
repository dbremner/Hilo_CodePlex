// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
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
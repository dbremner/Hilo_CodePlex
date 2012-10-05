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
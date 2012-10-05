#pragma once

#include "IResourceLoader.h"

namespace Hilo
{
    // The LocalResourceLoader provides a helper method that gets strings from the app's resource file.
    public ref class LocalResourceLoader sealed : public IResourceLoader
    {
    public:
        virtual Platform::String^ GetString(Platform::String^ value);
    };
}
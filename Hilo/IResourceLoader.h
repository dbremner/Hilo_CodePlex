#pragma once

namespace Hilo
{
    // The IResourceLoader class defines the interface used to access resource strings.
    // <snippet1900>
    public interface class IResourceLoader
    {
        Platform::String^ GetString(Platform::String^ value);
    };
    // </snippet1900>
}
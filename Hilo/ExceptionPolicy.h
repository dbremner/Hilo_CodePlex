#pragma once

namespace Hilo
{
    class ExceptionPolicy
    {
    public:
        virtual void HandleException(Platform::Exception^ exception) = 0;
    };
}
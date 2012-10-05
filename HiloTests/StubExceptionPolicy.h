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
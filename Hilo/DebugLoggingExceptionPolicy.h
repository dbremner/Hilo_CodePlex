#pragma once
#include "ExceptionPolicy.h"

namespace Hilo 
{
    class DebugLoggingExceptionPolicy: public ExceptionPolicy
    {
    public:
        virtual void HandleException(Platform::Exception^ exception);
    };
}
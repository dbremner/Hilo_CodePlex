#include "pch.h"
#include "StubExceptionPolicy.h"

using namespace HiloTests;
using namespace Platform;

void StubExceptionPolicy::HandleException(Exception^ exception)
{
    m_exception = exception;
}

Exception^ StubExceptionPolicy::GetSuppliedException()
{
    return m_exception;
}

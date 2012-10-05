#include "pch.h"
#include "DebugLoggingExceptionPolicy.h"

using namespace Hilo;
using namespace Platform;
using namespace std;

void DebugLoggingExceptionPolicy::HandleException(Exception^ exception)
{
    assert(exception != nullptr);
    wstringstream ss;
    ss << "[HR: " << exception->HResult << "] " << exception->Message->Data();
    OutputDebugString(ss.str().c_str());
}
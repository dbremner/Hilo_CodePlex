#include "pch.h"
#include "ExceptionPolicyFactory.h"
#include "DebugLoggingExceptionPolicy.h"

using namespace Hilo;
using namespace std;

shared_ptr<ExceptionPolicy> ExceptionPolicyFactory::GetCurrentPolicy()
{
    return std::make_shared<DebugLoggingExceptionPolicy>();
}

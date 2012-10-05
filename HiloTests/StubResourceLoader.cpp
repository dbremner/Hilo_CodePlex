#include "pch.h"
#include "StubResourceLoader.h"

using namespace HiloTests;
using namespace Platform;

String^ StubResourceLoader::GetString(String^ value)
{
    return StringToReturn;
}
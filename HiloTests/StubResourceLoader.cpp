//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "StubResourceLoader.h"

using namespace HiloTests;
using namespace Platform;

String^ StubResourceLoader::GetString(String^ value)
{
    return StringToReturn;
}
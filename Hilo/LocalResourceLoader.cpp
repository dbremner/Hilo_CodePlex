//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "LocalResourceLoader.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::ApplicationModel::Resources;

String^ LocalResourceLoader::GetString(String^ value)
{
    auto loader = ref new ResourceLoader();
    return loader->GetString(value);
}
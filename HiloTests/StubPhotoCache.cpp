//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "StubPhotoCache.h"
#include "..\Hilo\IPhoto.h"

using namespace Hilo;
using namespace HiloTests;

void StubPhotoCache::InsertPhoto(IPhoto^ photo)
{
}

IPhoto^ StubPhotoCache::GetForYearAndMonth(int year, int month)
{
    return nullptr;
}
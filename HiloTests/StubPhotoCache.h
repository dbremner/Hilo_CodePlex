//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once
#include "..\Hilo\IPhotoCache.h"

namespace HiloTests
{
    ref class StubPhotoCache sealed : public Hilo::IPhotoCache
    {
    public:
        virtual void InsertPhoto(Hilo::IPhoto^ photo);
        virtual Hilo::IPhoto^ GetForYearAndMonth(int year, int month);
    };
}
//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "PhotoGroupData.h"

namespace Hilo
{
    interface class IPhotoGroup;
    interface class IPhoto;

    class MonthGroupQuery 
    {
    public:
        virtual concurrency::task<PhotoGroupData> GetPhotoDataForMonthGroup(IPhotoGroup^ photoGroup, unsigned int maxNumberOfItems, concurrency::cancellation_token token) = 0;
    };
}
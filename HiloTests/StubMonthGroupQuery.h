//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "..\Hilo\MonthGroupQuery.h"

namespace HiloTests
{
    class StubMonthGroupQuery : public Hilo::MonthGroupQuery
    {
    public:
        virtual concurrency::task<Hilo::PhotoGroupData> GetPhotoDataForMonthGroup(Hilo::IPhotoGroup^ photoGroup, unsigned int maxNumberOfItems, concurrency::cancellation_token token);

        bool GetHasBeenCalled();
        void SetPhotoToReturn(Hilo::IPhoto^ photo);
    private:
        bool m_called;
        Hilo::IPhoto^ m_photoToReturn;
    };
}

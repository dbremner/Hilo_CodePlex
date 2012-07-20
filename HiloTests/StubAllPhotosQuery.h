//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "..\Hilo\AllPhotosQuery.h"

namespace HiloTests
{
    class StubAllPhotosQuery : public Hilo::AllPhotosQuery
    {
    public:
        virtual concurrency::task<Windows::Foundation::Collections::IVectorView<Hilo::IPhoto^>^> GetPhotosForDateRangeQueryAsync(Platform::String^ dateRangeQuery, concurrency::cancellation_token token);
        virtual void AddObserver(const std::function<void() > callback);
        virtual void RemoveObserver();

        bool GetHasBeenCalled();
        bool GetObserved();
        void SetPhotoToReturn(Hilo::IPhoto^ photo);
    private:
        bool m_called;
        bool m_observed;
        Hilo::IPhoto^ m_photoToReturn;
    };
}

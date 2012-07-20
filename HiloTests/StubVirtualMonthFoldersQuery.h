//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "..\Hilo\VirtualMonthFoldersQuery.h"

namespace HiloTests
{
    class StubVirtualMonthFoldersQuery : public Hilo::VirtualMonthFoldersQuery
    {
    public:
        virtual concurrency::task<Windows::Foundation::Collections::IVectorView<Hilo::IPhotoGroup^>^> GetMonthGroupedPhotosWithCacheAsync(std::shared_ptr<Hilo::PhotoCache> photoCache, concurrency::cancellation_token token);
        virtual void AddObserver(const std::function<void() > callback);
        virtual void RemoveObserver();

        bool GetHasBeenCalled();
        bool GetObserved();
    private:
        bool m_called;
        bool m_observed;
    };
}

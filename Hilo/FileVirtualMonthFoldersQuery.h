//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "VirtualMonthFoldersQuery.h"
#include "QueryChange.h"

namespace Hilo
{
    class ExceptionPolicy;

    class FileVirtualMonthFoldersQuery : public VirtualMonthFoldersQuery
    {
    public:
        FileVirtualMonthFoldersQuery(std::shared_ptr<ExceptionPolicy> exceptionPolicy);

        virtual concurrency::task<Windows::Foundation::Collections::IVectorView<IPhotoGroup^>^> GetMonthGroupedPhotosWithCacheAsync(std::shared_ptr<PhotoCache> photoCache, concurrency::cancellation_token token);
        virtual void AddObserver(const std::function<void() > callback);
        virtual void RemoveObserver();

    private:
        QueryChange^ m_pictureHubGroupQueryChange;
        std::function<void()> m_callback;
        std::shared_ptr<ExceptionPolicy> m_exceptionPolicy;
    };
}
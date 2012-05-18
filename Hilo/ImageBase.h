//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "ViewModelBase.h"

namespace Hilo
{
    interface class IExceptionPolicy;

#pragma warning(push)
#pragma warning(disable: 4449)
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class ImageBase : public ViewModelBase
    {
    public:
        ImageBase(IExceptionPolicy^ exceptionPolicy);
        
    protected:
        Windows::Foundation::IAsyncAction^ SaveImageAsync(Windows::Storage::BulkAccess::FileInformation^ file, Windows::Storage::Streams::IRandomAccessStream^ ras);

    private:
        concurrency::task<void> SaveImageAsyncImpl(Windows::Storage::BulkAccess::FileInformation^ file, Windows::Storage::Streams::IRandomAccessStream^ ras);
    };
#pragma warning(pop)
}

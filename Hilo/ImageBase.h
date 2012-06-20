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
    internal:
        ImageBase(IExceptionPolicy^ exceptionPolicy);
        
    protected:
        Windows::Foundation::IAsyncOperation<Windows::Storage::StorageFile^>^ GetFileNameFromFileSavePickerAsync(Platform::String^ fileType);
        Windows::Foundation::IAsyncAction^ SaveImageAsync(Windows::Storage::StorageFile^ file, Windows::Storage::Streams::IRandomAccessStream^ ras);

    private:
        concurrency::task<Windows::Storage::StorageFile^> GetFileNameFromFileSavePickerAsyncImpl(Platform::String^ fileType);
        concurrency::task<void> SaveImageAsyncImpl(Windows::Storage::StorageFile^ file, Windows::Storage::Streams::IRandomAccessStream^ ras);
    };
#pragma warning(pop)
}

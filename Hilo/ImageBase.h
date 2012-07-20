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
    interface class IPhoto;
    class ExceptionPolicy;

    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class ImageBase : public ViewModelBase
    {

    private protected:
        IPhoto^ m_photo;

    internal:
        ImageBase(std::shared_ptr<ExceptionPolicy> exceptionPolicy);

        concurrency::task<Windows::Storage::Streams::IRandomAccessStreamWithContentType^> GetStreamWithFailCheck();
        concurrency::task<Windows::Storage::StorageFile^> GetFileNameFromFileSavePickerAsync(Platform::String^ fileType);
        concurrency::task<void> SaveImageAsync(Windows::Storage::StorageFile^ file, Windows::Storage::Streams::IRandomAccessStream^ ras);
    };
}

//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "Common\BindableBase.h"
#include <ppltasks.h>

namespace Hilo
{
    ref class Photo;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class PhotoGroup sealed : public BindableBase
    {
    public:
        PhotoGroup(Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVectorView<Windows::Storage::BulkAccess::FileInformation^>^>^ task);

        property Platform::String^ Title 
        { 
            Platform::String^ get(); 
            void set(Platform::String^ value); 
        }

        property Platform::Object^ Items
        {
            Platform::Object^ get();
        }

    private:
        Platform::String^ m_title;
        concurrency::task<Windows::Foundation::Collections::IVectorView<Windows::Storage::BulkAccess::FileInformation^>^> m_task;
        Platform::Collections::Vector<Platform::Object^>^ m_photos;
    };
}

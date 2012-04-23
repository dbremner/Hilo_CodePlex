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

namespace Hilo
{
    ref class Photo;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class PhotoGroup sealed : public BindableBase
    {
    public:
        PhotoGroup(Windows::Storage::IStorageFolder^ storagefolder);
        PhotoGroup(Windows::Storage::IStorageFolder^ storagefolder, Platform::String^ title);

        property Platform::String^ Title 
        { 
            Platform::String^ get();
        }

        property Platform::Object^ Items
        {
            Platform::Object^ get();
        }

    private:
        Platform::String^ m_title;
        Windows::Storage::IStorageFolder^ m_storageFolder;
        Platform::Collections::Vector<Platform::Object^>^ m_photos;
    };
}

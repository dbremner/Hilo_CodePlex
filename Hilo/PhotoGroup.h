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
        PhotoGroup(Windows::Storage::IStorageFolder^ storagefolder, bool supportSpan);
        PhotoGroup(Windows::Storage::IStorageFolder^ storagefolder, bool supportSpan, Platform::String^ title);

        operator Windows::Storage::IStorageFolder^ ();

        property Platform::String^ Title 
        { 
            Platform::String^ get();
        }

        property Windows::Foundation::Collections::IObservableVector<Platform::Object^>^ Items
        {
            Windows::Foundation::Collections::IObservableVector<Platform::Object^>^ get();
        }

    private:
        Windows::Storage::IStorageFolder^ m_storageFolder;
        bool m_supportSpan;
        Platform::String^ m_title;
        Platform::Collections::Vector<Platform::Object^>^ m_photos;
    };
}

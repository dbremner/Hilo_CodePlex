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
#include "IPhotoGroup.h"

namespace Hilo
{
    ref class Photo;
    ref class PhotoCache;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class MonthGroup sealed : public BindableBase, public IPhotoGroup
    {
    public:
        MonthGroup(Windows::Storage::IStorageFolder^ storagefolder, PhotoCache^ photoCache);

        virtual operator Windows::Storage::IStorageFolder^ ();

        property Platform::String^ Title 
        { 
            virtual Platform::String^ get();
        }

        property Windows::Foundation::Collections::IObservableVector<Platform::Object^>^ Items
        {
            virtual Windows::Foundation::Collections::IObservableVector<Platform::Object^>^ get();
        }

    private:
        Windows::Storage::IStorageFolder^ m_storageFolder;
        Platform::String^ m_title;
        Platform::Collections::Vector<Platform::Object^>^ m_photos;
        Platform::WeakReference m_weakPhotoCache;
    };
}

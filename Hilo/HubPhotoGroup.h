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
    interface class IExceptionPolicy;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class HubPhotoGroup sealed : public BindableBase, public IPhotoGroup
    {
    public:
        HubPhotoGroup(Platform::String^ title, Platform::String^ emptyTitle, Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVectorView<Windows::Storage::BulkAccess::FileInformation^>^>^ task, IExceptionPolicy^ exceptionPolicy);

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
        Platform::String^ m_title;
        Platform::String^ m_emptyTitle;
        concurrency::task<Windows::Foundation::Collections::IVectorView<Windows::Storage::BulkAccess::FileInformation^>^> m_task;
        Platform::Collections::Vector<Platform::Object^>^ m_photos;
        bool m_retrievedPhotos;
        IExceptionPolicy^ m_exceptionPolicy;
    };
}

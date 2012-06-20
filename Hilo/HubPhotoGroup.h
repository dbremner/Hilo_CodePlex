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
    interface class IPhoto;
    interface class IRepository;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class HubPhotoGroup sealed : public Common::BindableBase, public IPhotoGroup
    {
    public:
        HubPhotoGroup(Platform::String^ title, Platform::String^ emptyTitle, IRepository^ repository, IExceptionPolicy^ exceptionPolicy);
        ~HubPhotoGroup();

        property Platform::String^ Title 
        { 
            virtual Platform::String^ get(); 
        }

        property Windows::Foundation::Collections::IObservableVector<IPhoto^>^ Items
        {
            virtual Windows::Foundation::Collections::IObservableVector<IPhoto^>^ get();
        }

    internal:
        concurrency::task<void> QueryPhotosAsync();

    private:
        Platform::String^ m_title;
        Platform::String^ m_emptyTitle;
        Platform::Collections::Vector<IPhoto^>^ m_photos;
        bool m_retrievedPhotos;
        Windows::Storage::Search::IStorageQueryResultBase^ m_queryResult;
        IExceptionPolicy^ m_exceptionPolicy;
        IRepository^ m_repository;
        Windows::Foundation::EventRegistrationToken m_dataToken;

        void OnDataChanged();
    };
}

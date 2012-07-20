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
    interface class IPhoto;
    class ExceptionPolicy;
    class PictureHubGroupQuery;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class HubPhotoGroup sealed : public Common::BindableBase, public IPhotoGroup
    {
    internal:
        HubPhotoGroup(Platform::String^ title, Platform::String^ emptyTitle, std::shared_ptr<PictureHubGroupQuery> query, std::shared_ptr<ExceptionPolicy> exceptionPolicy);
        concurrency::task<void> QueryPhotosAsync();

    public:
        virtual ~HubPhotoGroup();

        property Platform::String^ Title 
        { 
            virtual Platform::String^ get(); 
        }

        property Windows::Foundation::Collections::IObservableVector<IPhoto^>^ Items
        {
            virtual Windows::Foundation::Collections::IObservableVector<IPhoto^>^ get();
        }

    private:
        Platform::String^ m_title;
        Platform::String^ m_emptyTitle;
        std::shared_ptr<PictureHubGroupQuery> m_query;
        std::shared_ptr<ExceptionPolicy> m_exceptionPolicy;
        Platform::Collections::Vector<IPhoto^>^ m_photos;
        bool m_retrievedPhotos;
        bool m_receivedChangeWhileRunning;
        bool m_runningQuery;

        void OnDataChanged();
    };
}

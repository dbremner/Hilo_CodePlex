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
    interface class IPhotoCache;
    interface class IExceptionPolicy;
    interface class IPhoto;
    interface class IRepository;
    interface class IQueryOperation;
    interface class IResourceLoader;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class MonthGroup sealed : public Common::BindableBase, public IPhotoGroup
    {
    public:
        MonthGroup(Platform::String^ title, IPhotoCache^ photoCache, IRepository^ repository, IQueryOperation^ queryOperation, IExceptionPolicy^ exceptionPolicy);
        ~MonthGroup();
        
        property Platform::String^ Title 
        { 
            virtual Platform::String^ get();
        }

        property Windows::Foundation::Collections::IObservableVector<IPhoto^>^ Items
        {
            virtual Windows::Foundation::Collections::IObservableVector<IPhoto^>^ get();
        }

        property bool HasPhotos
        {
            bool get();
        }

    internal:
        concurrency::task<void> QueryPhotosAsync();

    private:
        Platform::String^ m_title;
        Platform::WeakReference m_weakPhotoCache;
        IRepository^ m_repository;
        IQueryOperation^ m_queryOperation;
        IExceptionPolicy^ m_exceptionPolicy;
        Platform::Collections::Vector<IPhoto^>^ m_photos;
        unsigned int m_count;
        Windows::Foundation::EventRegistrationToken m_dataToken;

        void OnDataChanged();
    };
}

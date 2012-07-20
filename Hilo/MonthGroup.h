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
    class ExceptionPolicy;
    class PhotoCache;
    class MonthGroupQuery;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class MonthGroup sealed : public Common::BindableBase, public IPhotoGroup
    {
    internal:
        MonthGroup(Platform::String^ title, std::shared_ptr<PhotoCache> photoCache, std::shared_ptr<MonthGroupQuery> query, std::shared_ptr<ExceptionPolicy> exceptionPolicy);     
        concurrency::task<void> QueryPhotosAsync();

    public:
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

    private:
        Platform::String^ m_title;
        std::weak_ptr<PhotoCache> m_weakPhotoCache;
        std::shared_ptr<MonthGroupQuery> m_query;
        std::shared_ptr<ExceptionPolicy> m_exceptionPolicy;
        Platform::Collections::Vector<IPhoto^>^ m_photos;
        unsigned int m_count;
        bool m_runningQuery;
    };
}

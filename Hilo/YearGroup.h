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
    interface class IExceptionPolicy;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class YearGroup sealed : public BindableBase, public IPhotoGroup
    {
    public:
        YearGroup(Windows::Storage::IStorageFolder^ storagefolder, IExceptionPolicy^ exceptionPolicy);

        virtual operator Windows::Storage::IStorageFolder^ ();

        property Platform::String^ Title
        { 
            virtual Platform::String^ get();
        }

        property unsigned int Year
        {
            unsigned int get();
        }

        property Windows::Foundation::Collections::IObservableVector<Platform::Object^>^ Items
        {
            virtual Windows::Foundation::Collections::IObservableVector<Platform::Object^>^ get();
        }

    private:
        Windows::Storage::IStorageFolder^ m_storageFolder;
        Platform::Collections::Vector<Platform::Object^>^ m_months;
        unsigned int m_year;
        IExceptionPolicy^ m_exceptionPolicy;
    };
}

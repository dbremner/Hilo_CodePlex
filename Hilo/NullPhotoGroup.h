//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "IPhotoGroup.h"

namespace Hilo
{
    interface class IPhoto;

    ref class NullPhotoGroup : public IPhotoGroup
    {
    public:
        virtual operator Windows::Storage::IStorageFolder^ ()
        {
            return nullptr;
        }

        property Platform::String^ Title 
        { 
            virtual Platform::String^ get()
            {
                return "";
            }
        }

        property Windows::Foundation::Collections::IObservableVector<IPhoto^>^ Items
        {
            virtual Windows::Foundation::Collections::IObservableVector<IPhoto^>^ get()
            {
                return nullptr;
            }
        }
    };
}
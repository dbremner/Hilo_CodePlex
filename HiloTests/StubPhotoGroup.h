//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "..\Hilo\IPhotoGroup.h"

namespace HiloTests
{
    public ref class StubPhotoGroup sealed : public Hilo::IPhotoGroup
    {
    public:
        StubPhotoGroup(Platform::String^ name);

        virtual operator Windows::Storage::IStorageFolder^ ();

        property Platform::String^ Title 
        { 
            virtual Platform::String^ get();
        }

        property Windows::Foundation::Collections::IObservableVector<Platform::Object^>^ Items
        {
            virtual Windows::Foundation::Collections::IObservableVector<Platform::Object^>^ get();
        }
    };
}

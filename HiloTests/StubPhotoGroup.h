// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#pragma once

#include "..\Hilo\IPhotoGroup.h"

namespace HiloTests
{
    ref class StubPhotoGroup sealed : public Hilo::IPhotoGroup
    {
    public:
        StubPhotoGroup(Platform::String^ name);

        property Platform::String^ Title 
        { 
            virtual Platform::String^ get();
        }

        property Windows::Foundation::Collections::IObservableVector<Hilo::IPhoto^>^ Items
        {
            virtual Windows::Foundation::Collections::IObservableVector<Hilo::IPhoto^>^ get();
        }

    internal:
        virtual operator Windows::Storage::IStorageFolder^ ();
    };
}

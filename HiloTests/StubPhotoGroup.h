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

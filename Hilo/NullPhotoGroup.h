#pragma once

#include "IPhotoGroup.h"

namespace Hilo
{
    interface class IPhoto;

    // Instances of the NullPhotoGroup class are placeholders used by photos that are not part of any group.
    ref class NullPhotoGroup : public IPhotoGroup
    {
    
    internal:
        virtual operator Windows::Storage::IStorageFolder^ ()
        {
            return nullptr;
        }

    public:
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
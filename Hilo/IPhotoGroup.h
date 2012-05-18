//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

namespace Hilo
{
    public interface class IPhotoGroup
    {
        operator Windows::Storage::IStorageFolder^ ();

        property Platform::String^ Title 
        { 
            Platform::String^ get();
        }

        property Windows::Foundation::Collections::IObservableVector<Platform::Object^>^ Items
        {
            Windows::Foundation::Collections::IObservableVector<Platform::Object^>^ get();
        }
    };
}

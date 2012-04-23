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

namespace Hilo
{
    [Windows::UI::Xaml::Data::Bindable]
    public ref class BrowserPhotoGroup sealed : public BindableBase
    {
    public:
        BrowserPhotoGroup(Windows::Storage::BulkAccess::FolderInformation^ folderInformation);

        property Platform::String^ Title
        { 
            Platform::String^ get(); 
        }

        property Platform::Object^ Items
        {
            Platform::Object^ get();
        }

    private:
        Windows::Storage::BulkAccess::FolderInformation^ m_folderInformation;
        Platform::Object^ m_photos;
    };
}

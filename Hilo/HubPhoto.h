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
    public ref class HubPhoto sealed : public Hilo::BindableBase
    {

    public:
        HubPhoto(Windows::Storage::BulkAccess::FileInformation^ file);

        operator Windows::Storage::BulkAccess::FileInformation^ ();

        property Platform::String^ Name { Platform::String^ get(); }

        property Windows::UI::Xaml::Media::Imaging::BitmapImage^ Thumbnail 
        { 
            Windows::UI::Xaml::Media::Imaging::BitmapImage^ get();
        }

        property int ColumnSpan 
        {
            int get();
            void set(int value);
        }

        property int RowSpan
        {
            int get();
            void set(int value);
        }

    private:
        Windows::Storage::BulkAccess::FileInformation^ m_fileInfo;
        Windows::Storage::StorageFile^ m_storageFile;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ m_thumbnail;
        int m_columnSpan;
        int m_rowSpan;
    };

}
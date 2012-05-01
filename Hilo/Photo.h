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
    ref class PhotoGroup;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class Photo sealed : public BindableBase
    {

    public:
        Photo(Windows::Storage::BulkAccess::FileInformation^ file, PhotoGroup^ photoGroup);

        operator Windows::Storage::IStorageFile^ ();

        property Platform::String^ Name { Platform::String^ get(); }

        property Windows::UI::Xaml::Media::Imaging::BitmapImage^ Thumbnail 
        { 
            Windows::UI::Xaml::Media::Imaging::BitmapImage^ get();
        }

        property PhotoGroup^ Group
        {
            PhotoGroup^ get();
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
        Platform::WeakReference m_weakPhotoGroup;
        int m_columnSpan;
        int m_rowSpan;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ m_thumbnail;
    };

}
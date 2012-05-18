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
#include "IResizable.h"

namespace Hilo 
{
    interface class IPhotoGroup;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class Photo sealed : public BindableBase, public IResizable
    {
    public:
        Photo(Windows::Storage::BulkAccess::FileInformation^ file, IPhotoGroup^ photoGroup);

        operator Windows::Storage::IStorageFile^ ();

        operator Windows::Storage::BulkAccess::FileInformation^ ();

        property Platform::String^ Name { Platform::String^ get(); }

        property Windows::UI::Xaml::Media::Imaging::BitmapImage^ Thumbnail 
        { 
            Windows::UI::Xaml::Media::Imaging::BitmapImage^ get();
        }

        property IPhotoGroup^ Group
        {
            IPhotoGroup^ get();
        }

        property int ColumnSpan 
        {
            virtual int get();
            virtual void set(int value);
        }

        property int RowSpan
        {
            virtual int get();
            virtual void set(int value);
        }

        property Platform::Object^ FileName
        {
            Platform::Object^ get();
        }

        property Platform::Object^ FileDateCreated
        {
            Platform::Object^ get();
        }

        property Platform::Object^ FileDateModified
        {
            Platform::Object^ get();
        }

    private:
        Windows::Storage::BulkAccess::FileInformation^ m_fileInfo;
        Platform::WeakReference m_weakPhotoGroup;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ m_thumbnail;
        int m_columnSpan;
        int m_rowSpan;
    };

}
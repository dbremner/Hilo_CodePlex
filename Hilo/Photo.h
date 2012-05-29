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
    interface class IExceptionPolicy;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class Photo sealed : public BindableBase, public IResizable
    {
    public:
        Photo(Windows::Storage::BulkAccess::FileInformation^ file, IPhotoGroup^ photoGroup, IExceptionPolicy^ exceptionPolicy);

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

    private:
        Windows::Storage::BulkAccess::FileInformation^ m_fileInfo;
        Platform::WeakReference m_weakPhotoGroup;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ m_thumbnail;
        IExceptionPolicy^ m_exceptionPolicy;
        int m_columnSpan;
        int m_rowSpan;

        void OnThumbnailUpdated(Windows::Storage::BulkAccess::IStorageItemInformation^ sender, Platform::Object^ e);
    };

}
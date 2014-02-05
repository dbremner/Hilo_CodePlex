// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#pragma once

#include "IResizable.h"
#include "IPhoto.h"

namespace Hilo 
{
    interface class IPhotoGroup;
    interface class IPhotoImage;
    class ExceptionPolicy;

    // The Photo class provides data used by XAML image controls.
    [Windows::UI::Xaml::Data::Bindable]
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class Photo sealed : public IResizable, public IPhoto, public Windows::UI::Xaml::Data::INotifyPropertyChanged
    {
    internal:
        Photo(Windows::Storage::StorageFile^ file, IPhotoGroup^ photoGroup, std::shared_ptr<ExceptionPolicy> exceptionPolicy);

    public:
        virtual ~Photo();

        virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;

        property IPhotoGroup^ Group
        {
            virtual IPhotoGroup^ get();
        }
                
        property Platform::String^ Name 
        { 
            virtual Platform::String^ get(); 
        }

        property Platform::String^ Path
        {
            virtual Platform::String^ get();
        }

        property Windows::UI::Xaml::Media::Imaging::BitmapImage^ Thumbnail 
        { 
            virtual Windows::UI::Xaml::Media::Imaging::BitmapImage^ get();
        }

        property bool IsInvalidThumbnail
        {
            virtual bool get();
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

        virtual IPhotoImage^ GetPhotoImage();
        virtual Windows::Foundation::IAsyncOperation<Windows::Foundation::DateTime>^ GetDateTakenAsync();

        property Windows::Storage::StorageFile^  File
        {
            Windows::Storage::StorageFile^ get();
        }

    private:
        // State of thumbnail get operation.
        enum UpdateStatus 
        { 
            Uninitialized, // Thumbnail get operation has not started to run.
            Running,       // Thumbnail get operation is currently running.
            Ready,         // A valid thumbnail has been retrieved.
            Error          // Thumbnail get operation has finished running without retrieving a thumbnail image.
        };

        Windows::Storage::StorageFile^ m_file;
        Platform::WeakReference m_weakPhotoGroup;
        UpdateStatus m_thumbnailStatus; 
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ m_thumbnail;
        std::shared_ptr<ExceptionPolicy> m_exceptionPolicy;
        int m_columnSpan;
        int m_rowSpan;

        concurrency::task<Windows::Foundation::DateTime> GetDateTakenAsyncInternal();
        void StartThumbnailGet();
        void UseDefaultThumbnail();
        void OnPropertyChanged(Platform::String^ propertyName);
    };
}
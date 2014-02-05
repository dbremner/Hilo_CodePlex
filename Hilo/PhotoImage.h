// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#pragma once

#include "IResizable.h"
#include "IPhotoImage.h"

namespace Hilo 
{
    interface class IPhotoGroup;
    class ExceptionPolicy;
    ref class Photo;

    // The PhotoImage class provides data used by XAML image controls that display full images (not just thumbnails).
    [Windows::UI::Xaml::Data::Bindable]
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class PhotoImage sealed : public IPhotoImage, public Windows::UI::Xaml::Data::INotifyPropertyChanged
    {
    internal:
        PhotoImage(Photo^ photo, std::shared_ptr<ExceptionPolicy> exceptionPolicy);

    public:
        virtual ~PhotoImage();

        virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;

        property Platform::String^ Name 
        { 
            virtual Platform::String^ get(); 
        }

        property Platform::String^ Path
        {
            virtual Platform::String^ get();
        }

        property Platform::String^ FormattedPath
        {
            virtual Platform::String^ get();
        }

        property Platform::String^ FileType
        {
            virtual Platform::String^ get();
        }
                
        property Platform::String^ FormattedDateTaken
        {
            virtual Platform::String^ get();
        }

        property Platform::String^ FormattedTimeTaken
        {
            virtual Platform::String^ get();
        }

        property Platform::String^ Resolution
        {
            virtual Platform::String^ get();
        }

        property uint64 FileSize
        {
            virtual uint64 get();
        }

        property Platform::String^ DisplayType
        {
            virtual Platform::String^ get();
        }

        property Windows::UI::Xaml::Media::Imaging::BitmapImage^ Thumbnail 
        { 
            virtual Windows::UI::Xaml::Media::Imaging::BitmapImage^ get();
        }

        property Windows::UI::Xaml::Media::Imaging::BitmapImage^ Image
        { 
            virtual Windows::UI::Xaml::Media::Imaging::BitmapImage^ get();
        }

        property bool IsInvalidThumbnail
        {
            virtual bool get();
        }

        virtual Windows::Foundation::IAsyncAction^ InitializeAsync();
        virtual Windows::Foundation::IAsyncOperation<Windows::Storage::Streams::IRandomAccessStreamWithContentType^>^ OpenReadAsync();
        virtual void ClearImageData();

    private:
        Photo^ m_photo;
        Windows::Foundation::DateTime m_dateTaken;
        Platform::String^ m_resolution;
        uint64 m_fileSize;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ m_image;
        Windows::Foundation::EventRegistrationToken m_imageFailedEventToken;
        Windows::Foundation::EventRegistrationToken m_photoPropertyChangedEventToken;
        std::shared_ptr<ExceptionPolicy> m_exceptionPolicy;

        concurrency::task<void> InitializeAsyncInternal();
        concurrency::task<void> InitializeFilePropertiesAsync();
        concurrency::task<void> InitializeImageAsync();

        void OnPhotoPropertyChanged(Platform::Object^ sender, Windows::UI::Xaml::Data::PropertyChangedEventArgs^ e);
        void OnImageFailedToOpen(Platform::Object^ sender, Windows::UI::Xaml::ExceptionRoutedEventArgs^ e);
        void OnPropertyChanged(Platform::String^ propertyName);
    };
}
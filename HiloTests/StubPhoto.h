//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "..\Hilo\IPhoto.h"

namespace HiloTests
{
    ref class StubPhoto sealed : public Hilo::IPhoto
    {
    public:
        property Hilo::IPhotoGroup^ Group
        {
            virtual Hilo::IPhotoGroup^ get();
        }

        property Platform::String^ Name 
        { 
            virtual Platform::String^ get(); 
        }
        
        property Platform::String^ Path
        {
            virtual Platform::String^ get();
            void set(Platform::String^ value);
        }

        property Platform::String^ FormattedPath
        {
            virtual Platform::String^ get();
        }
        
        property Platform::String^ FileType
        {
            virtual Platform::String^ get();
        }

        property Windows::Foundation::DateTime DateTaken
        {
            virtual Windows::Foundation::DateTime get();
            void set(Windows::Foundation::DateTime value);
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

        property unsigned long long FileSize
        {
            virtual unsigned long long get();
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

        virtual Windows::Foundation::IAsyncOperation<Windows::Storage::FileProperties::ImageProperties^>^ GetImagePropertiesAsync();
        virtual Windows::Foundation::IAsyncOperation<Windows::Storage::Streams::IRandomAccessStreamWithContentType^>^ OpenReadAsync();
        virtual void ClearImageData();

    private:
        Platform::String^ m_name;
        Platform::String^ m_fileType;
        Windows::Foundation::DateTime m_dateTaken;
        Platform::String^ m_path;
        Platform::String^ m_resolution;
        unsigned long long m_fileSize;
        Platform::String^ m_displayType;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ m_image;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ m_thumbnail;
        Windows::Storage::FileProperties::ImageProperties^ m_imageProperties;
        Windows::Storage::Streams::IRandomAccessStreamWithContentType^ m_randomAccessStream;
    };
}

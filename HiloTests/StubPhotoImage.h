﻿// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#pragma once

#include "..\Hilo\IPhotoImage.h"

namespace HiloTests
{
    ref class StubPhotoImage sealed : public Hilo::IPhotoImage
    {
    public:
        StubPhotoImage();
        StubPhotoImage(Platform::String^ path, Platform::String^ name);

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

        void SetDateTaken(Windows::Foundation::DateTime value);

    private:
        Platform::String^ m_name;
        Platform::String^ m_fileType;
        Windows::Foundation::DateTime m_dateTaken;
        Platform::String^ m_path;
        Platform::String^ m_resolution;
        uint64 m_fileSize;
        Platform::String^ m_displayType;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ m_image;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ m_thumbnail;
        Windows::Storage::Streams::IRandomAccessStreamWithContentType^ m_randomAccessStream;
        bool m_isInvalidThumbnail;
    };
}

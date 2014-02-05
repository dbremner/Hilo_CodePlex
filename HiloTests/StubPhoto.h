// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#pragma once

#include "..\Hilo\IPhoto.h"
#include "..\Hilo\IPhotoImage.h"

namespace HiloTests
{
    ref class StubPhoto sealed : public Hilo::IPhoto
    {
    public:
        StubPhoto();

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
        
        virtual Hilo::IPhotoImage^ GetPhotoImage();
        virtual Windows::Foundation::IAsyncOperation<Windows::Foundation::DateTime>^ GetDateTakenAsync();

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
        int m_columnSpan;
        int m_rowSpan;
    };
}

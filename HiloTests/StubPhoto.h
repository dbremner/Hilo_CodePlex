#pragma once

#include "..\Hilo\IPhoto.h"

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

        virtual Windows::Foundation::IAsyncOperation<Windows::Storage::Streams::IRandomAccessStreamWithContentType^>^ OpenReadAsync();
        virtual void ClearImageData();

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

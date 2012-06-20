//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

namespace Hilo
{
    interface class IPhotoGroup;

    public interface class IPhoto
    {
        property IPhotoGroup^ Group
        {
            IPhotoGroup^ get();
        }

        property Platform::String^ Name { Platform::String^ get(); }
        
        property Platform::String^ Path
        {
            Platform::String^ get();
        }

        property Platform::String^ FormattedPath
        {
            Platform::String^ get();
        }
        
        property Platform::String^ FileType
        {
            Platform::String^ get();
        }

        property Windows::Foundation::DateTime DateTaken
        {
            Windows::Foundation::DateTime get();
        }

        property Platform::String^ FormattedDateTaken
        {
            Platform::String^ get();
        }

        property Platform::String^ FormattedTimeTaken
        {
            Platform::String^ get();
        }

        property Platform::String^ Resolution
        {
            Platform::String^ get();
        }

        property unsigned long long FileSize
        {
            unsigned long long get();
        }

        property Platform::String^ DisplayType
        {
            Platform::String^ get();
        }

        property Windows::UI::Xaml::Media::Imaging::BitmapImage^ Thumbnail 
        { 
            Windows::UI::Xaml::Media::Imaging::BitmapImage^ get();
        }

        property Windows::UI::Xaml::Media::Imaging::BitmapImage^ Image
        { 
            Windows::UI::Xaml::Media::Imaging::BitmapImage^ get();
        }

        void ClearImageData();

        Windows::Foundation::IAsyncOperation<Windows::Storage::FileProperties::ImageProperties^>^ GetImagePropertiesAsync();
        Windows::Foundation::IAsyncOperation<Windows::Storage::Streams::IRandomAccessStreamWithContentType^>^ OpenReadAsync();
    };
}
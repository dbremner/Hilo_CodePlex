// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#pragma once

namespace Hilo
{
    interface class IPhotoGroup;
    interface class IPhotoImage;

    // The IPhoto class defines the signature of thumbnail images used by XAML.
    [Windows::Foundation::Metadata::WebHostHidden]
    public interface class IPhoto
    {
        property IPhotoGroup^ Group
        {
            IPhotoGroup^ get();
        }

        property Platform::String^ Name 
        { 
            Platform::String^ get(); 
        }

        property Platform::String^ Path
        {
            Platform::String^ get();
        }  

        property Windows::UI::Xaml::Media::Imaging::BitmapImage^ Thumbnail 
        { 
            Windows::UI::Xaml::Media::Imaging::BitmapImage^ get();
        }

        property bool IsInvalidThumbnail
        {
            bool get();
        }

        IPhotoImage^ GetPhotoImage();
        
        Windows::Foundation::IAsyncOperation<Windows::Foundation::DateTime>^ GetDateTakenAsync(); 
    };
}
//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "ImageBase.h"

namespace Hilo
{
    [Windows::UI::Xaml::Data::Bindable]
    public ref class CropImageViewModel sealed : public ImageBase
    {
    public:
        CropImageViewModel();
        ~CropImageViewModel();

        property int CropRectangleWidth { int get(); }
        property int CropRectangleHeight { int get(); }
        property Platform::Object^ CropRectangleMargin { Platform::Object^ get(); }
        property bool IsCropRectangleVisible { bool get(); }

        property Windows::UI::Xaml::Media::ImageSource^ Photo
        {
            Windows::UI::Xaml::Media::ImageSource^ get();
        }

        property Windows::UI::Xaml::Input::ICommand^ SaveCommand
        {
            Windows::UI::Xaml::Input::ICommand^ get();
        }

        property Windows::UI::Xaml::Input::ICommand^ CancelCommand
        {
            Windows::UI::Xaml::Input::ICommand^ get();
        }

        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

        void GetCropStartCoordinates(Windows::Foundation::Point onScreen, Windows::Foundation::Point relative, int pageTitleRowHeight);
        void CalculateCropRectangleCoordinates(Windows::Foundation::Point onScreen, Windows::Foundation::Point relative, Windows::UI::Xaml::Thickness imageMargin);
        void CropImage(double actualWidth);
    
    private:
        Windows::Storage::BulkAccess::FileInformation^ m_file;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ m_image;
        Windows::Storage::Streams::IRandomAccessStream^ m_randomAccessStream;
        Windows::UI::Xaml::Input::ICommand^ m_saveCommand;
        Windows::UI::Xaml::Input::ICommand^ m_cancelCommand;
        bool m_isCropRectangleVisible;
        int m_cropRectangleWidth;
        int m_cropRectangleHeight;
        int m_pageTitleRowHeight;
        Windows::Foundation::Point m_actualCropAnchorPoint;
        Windows::Foundation::Point m_actualCropEndPoint;
        Windows::Foundation::Point m_relativeCropAnchorPoint;
        Windows::Foundation::Point m_relativeCropEndPoint;
        Platform::Object^ m_cropRectangleMargin;

        // Image data
        Windows::Graphics::Imaging::BitmapDecoder^ m_decoder;
        Platform::Array<unsigned char, 1>^ m_sourcePixels;
        Platform::Array<unsigned char, 1>^ m_destinationPixels;
        int m_imageWidth;
        int m_imageHeight;
        int m_newImageWidth;
        int m_newImageHeight;

        // Member functions
        void PopulatePixelArray();

        // Member functions that implement Commands
        void SaveImage(Platform::Object^ parameter);
        void CancelCrop(Platform::Object^ parameter);
    };
}


//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "ViewModelBase.h"

namespace Hilo
{
    [Windows::UI::Xaml::Data::Bindable]
    public ref class EditImageViewModel sealed : public ViewModelBase
    {
    public:
        EditImageViewModel();
        ~EditImageViewModel();

        property Windows::UI::Xaml::Media::ImageSource^ Photo
        {
            Windows::UI::Xaml::Media::ImageSource^ get();
        }

        property bool IsCropRectangleVisible { bool get(); }
        property int CropRectangleWidth { int get(); }
        property int CropRectangleHeight { int get(); }
        property Platform::Object^ CropRectangleMargin { Platform::Object^ get(); }

        property double Angle { double get(); }

        // Commands
        property Windows::UI::Xaml::Input::ICommand^ RotateCommand;
        property Windows::UI::Xaml::Input::ICommand^ UndoEditCommand;
        property Windows::UI::Xaml::Input::ICommand^ CancelEditCommand;
        property Windows::UI::Xaml::Input::ICommand^ SaveEditCommand;
        //property Windows::UI::Xaml::Input::ICommand^ CropModeCommand;
        //property Windows::UI::Xaml::Input::ICommand^ RotateModeCommand;
        
        // Public member functions
        void CalculateRotationAngle(float delta);
        void CalculateRotationAngle(float x, float y, double width, double height);
        void GetCropStartCoordinates(Windows::Foundation::Point onScreen, Windows::Foundation::Point relative, int pageTitleRowHeight);
        void CalculateCropRectangleCoordinates(Windows::Foundation::Point onScreen, Windows::UI::Xaml::Thickness imageMargin);
        void CropImage(double actualWidth);

        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

    private:
        Windows::Storage::BulkAccess::FileInformation^ m_file;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ m_image;
        bool m_isCropRectangleVisible;
        int m_cropRectangleWidth;
        int m_cropRectangleHeight;
        bool m_isAppBarSticky;
        int m_pageTitleRowHeight;
        Windows::Foundation::Point m_actualCropAnchorPoint;
        Windows::Foundation::Point m_relativeCropAnchorPoint;
        Platform::Object^ m_cropRectangleMargin;

        // Image data
        Windows::Storage::Streams::IRandomAccessStream^ m_sourceImageStream;
        Windows::Storage::Streams::FileRandomAccessStream^ m_destinationImageStream;
        Platform::Array<unsigned char, 1>^ m_sourcePixels;
        Platform::Array<unsigned char, 1>^ m_destinationPixels;
        int m_imageWidth;
        int m_imageHeight;
        int m_newImageWidth;
        int m_newImageHeight;
        double m_angle;
        Windows::Graphics::Imaging::BitmapDecoder^ m_decoder;

        // Member functions
        void PopulatePixelArray(Windows::Storage::Streams::IRandomAccessStream^ stream);
        void RotateImage();
     
        // Member functions that implement Commands
        void Rotate90(Platform::Object^ parameter);
        void Undo(Platform::Object^ parameter);
        void Cancel(Platform::Object^ parameter);
        void SaveImage(Platform::Object^ parameter);
    };
}




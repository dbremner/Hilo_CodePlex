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
    interface class IExceptionPolicy;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class RotateImageViewModel sealed : public ImageBase
    {
    public:
        RotateImageViewModel(IExceptionPolicy^ exceptionPolicy);

        property Windows::UI::Xaml::Media::ImageSource^ Photo
        {
            Windows::UI::Xaml::Media::ImageSource^ get();
        }

        property Windows::UI::Xaml::Input::ICommand^ RotateCommand
        {
            Windows::UI::Xaml::Input::ICommand^ get();
        }

        property Windows::UI::Xaml::Input::ICommand^ SaveCommand
        {
            Windows::UI::Xaml::Input::ICommand^ get();
        }

        property Windows::UI::Xaml::Input::ICommand^ CancelCommand
        {
            Windows::UI::Xaml::Input::ICommand^ get();
        }

        ///<summary>Returns the rotation angle for image display.</summary>
        property double RotationAngle 
        {
            double get();
            void set(double value);
        }

        property Platform::Object^ FileName
        {
            Platform::Object^ get();
        }

        property Platform::Object^ FileDateCreated
        {
            Platform::Object^ get();
        }

        property Platform::Object^ FileDateModified
        {
            Platform::Object^ get();
        }

        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

        void Initialize(Windows::Storage::BulkAccess::FileInformation^ image);

        void EndRotation();

    private:
        Windows::Storage::BulkAccess::FileInformation^ m_file;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ m_image;
        Windows::Storage::Streams::IRandomAccessStream^ m_randomAccessStream;
        Windows::UI::Xaml::Input::ICommand^ m_rotateCommand;
        Windows::UI::Xaml::Input::ICommand^ m_saveCommand;
        Windows::UI::Xaml::Input::ICommand^ m_cancelCommand;
        bool m_isSaving;
        double m_rotationAngle;

        concurrency::task<void> RotateImageViewModel::DoRotate(double angle);

        void Rotate90(Platform::Object^ parameter);
        void SaveImage(Platform::Object^ parameter);
        void CancelRotate(Platform::Object^ parameter);
    };
}

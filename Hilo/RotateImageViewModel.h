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
    public ref class RotateImageViewModel sealed : public ImageBase
    {
    public:
        RotateImageViewModel();

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

        property bool IsRotating
        {
            bool get();
        }

        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

    private:
        Windows::Storage::BulkAccess::FileInformation^ m_file;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ m_image;
        Windows::Storage::Streams::IRandomAccessStream^ m_randomAccessStream;
        Windows::UI::Xaml::Input::ICommand^ m_rotateCommand;
        Windows::UI::Xaml::Input::ICommand^ m_saveCommand;
        Windows::UI::Xaml::Input::ICommand^ m_cancelCommand;
        concurrency::task<void> m_head;
        bool m_isRotating;

        void Rotate90(Platform::Object^ parameter);
        void SaveImage(Platform::Object^ parameter);
        void CancelRotate(Platform::Object^ parameter);
    };
}

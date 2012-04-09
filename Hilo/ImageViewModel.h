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
    public ref class ImageViewModel sealed : public ViewModelBase
    {
    public:
        ImageViewModel();

        // Properties
        property Platform::Object^ Photos 
        { 
            Platform::Object^ get(); 
        }

        property Windows::UI::Xaml::Media::ImageSource^ Photo
        {
            Windows::UI::Xaml::Media::ImageSource^ get();
        }

        property Windows::Storage::BulkAccess::FileInformation^ SelectedItem
        {
            Windows::Storage::BulkAccess::FileInformation^ get();
            void set(Windows::Storage::BulkAccess::FileInformation^ value);
        }

        property Windows::UI::Xaml::Input::ICommand^ EditImageCommand
        {
            Windows::UI::Xaml::Input::ICommand^ get();
        }

        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

    private:
        Windows::Storage::BulkAccess::FileInformation^ m_photo;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ m_image;
        Platform::Object^ m_photos;
        Windows::UI::Xaml::Input::ICommand^ m_editImageCommand;

        void EditImage(Platform::Object^ parameter);
    };
}

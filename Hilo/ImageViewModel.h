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
    interface class IExceptionPolicy;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class ImageViewModel sealed : public ViewModelBase
    {
    public:
        ImageViewModel(IExceptionPolicy^ exceptionPolicy);

        property Windows::Foundation::Collections::IObservableVector<Platform::Object^>^ Photos
        { 
            Windows::Foundation::Collections::IObservableVector<Platform::Object^>^ get();
        }

        property Windows::Storage::BulkAccess::FileInformation^ SelectedItem
        {
            Windows::Storage::BulkAccess::FileInformation^ get();
            void set(Windows::Storage::BulkAccess::FileInformation^ value);
        }

        property Platform::String^ MonthAndYear
        {
            Platform::String^ get();
        }

        property Windows::UI::Xaml::Input::ICommand^ CropImageCommand
        {
            Windows::UI::Xaml::Input::ICommand^ get();
        }

        property Windows::UI::Xaml::Input::ICommand^ RotateImageCommand
        {
            Windows::UI::Xaml::Input::ICommand^ get();
        }

        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

        void Initialize(Platform::Object^ parameter);

    private:
        Windows::Foundation::DateTime m_fileDate;
        Platform::String^ m_filePath;
        Platform::String^ m_query;
        Windows::Storage::BulkAccess::FileInformation^ m_photo;
        Platform::Collections::Vector<Platform::Object^>^ m_photos;
        Windows::UI::Xaml::Input::ICommand^ m_cropImageCommand;
        Windows::UI::Xaml::Input::ICommand^ m_rotateImageCommand;
        concurrency::cancellation_token_source m_cts;

        void CropImage(Platform::Object^ parameter);
        void RotateImage(Platform::Object^ parameter);

        //concurrency::task<void> GetPhotoAsync();
    };
}

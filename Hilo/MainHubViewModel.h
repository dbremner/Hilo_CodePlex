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
    ref class HubPhotoGroup;
    interface class IExceptionPolicy;
    ref class Photo;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class MainHubViewModel sealed : public ViewModelBase
    {
    public:
        MainHubViewModel(Windows::Foundation::Collections::IObservableVector<HubPhotoGroup^>^ photoGroups, IExceptionPolicy^ exceptionPolicy);

        property Platform::Object^ PhotoGroups
        {
            Platform::Object^ get();
        }

        property Windows::UI::Xaml::Input::ICommand^ NavigateToPicturesCommand
        {
            Windows::UI::Xaml::Input::ICommand^ get();
        }

        property Windows::UI::Xaml::Input::ICommand^ CropImageCommand
        {
            Windows::UI::Xaml::Input::ICommand^ get();
        }

        property Windows::UI::Xaml::Input::ICommand^ RotateImageCommand
        {
            Windows::UI::Xaml::Input::ICommand^ get();
        }

        property bool IsAppBarEnabled 
        { 
            bool get(); 
            void set(bool value);
        }

        property Photo^ SelectedItem
        {
            Photo^ get();
            void set(Photo^ value);
        }

        void NavigateToImageView(Photo^ photo);

    private:
        Windows::Foundation::Collections::IObservableVector<HubPhotoGroup^>^ m_photoGroups;
        Windows::UI::Xaml::Input::ICommand^ m_navigateToPicturesCommand;
        Windows::UI::Xaml::Input::ICommand^ m_cropImageCommand;
        Windows::UI::Xaml::Input::ICommand^ m_rotateImageCommand;
        bool m_isAppBarEnabled;
        Photo^ m_photo;
        HubPhotoGroup^ m_pictureGroup;

        void NavigateToPictures(Platform::Object^ parameter);
        bool CanNavigateToPictures(Platform::Object^ parameter);
        void CropImage(Platform::Object^ parameter);
        bool CanCropImage(Platform::Object^ parameter);
        void RotateImage(Platform::Object^ parameter);
        bool CanCropOrRotateImage(Platform::Object^ parameter);
    };
}

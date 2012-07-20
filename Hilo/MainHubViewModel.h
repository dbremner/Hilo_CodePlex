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
    interface class IPhoto;
    class ExceptionPolicy;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class MainHubViewModel sealed : public ViewModelBase
    {
    internal:
        MainHubViewModel(Windows::Foundation::Collections::IObservableVector<HubPhotoGroup^>^ photoGroups, std::shared_ptr<ExceptionPolicy> exceptionPolicy);
        
    public:
        virtual ~MainHubViewModel();

        property Windows::Foundation::Collections::IObservableVector<HubPhotoGroup^>^ PhotoGroups
        {
            Windows::Foundation::Collections::IObservableVector<HubPhotoGroup^>^ get();
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

        // These are really IPhoto^ but XAML cannot convert any underlying
        // real object (e.g. Photo) back to IPhoto which results in strange
        // side effects (such as the AppBar not working).
        property Platform::Object^ SelectedItem
        {
            Platform::Object^ get();
            void set(Platform::Object^ value);
        }

    private:
        Windows::Foundation::Collections::IObservableVector<HubPhotoGroup^>^ m_photoGroups;
        Windows::UI::Xaml::Input::ICommand^ m_navigateToPicturesCommand;
        Windows::UI::Xaml::Input::ICommand^ m_cropImageCommand;
        Windows::UI::Xaml::Input::ICommand^ m_rotateImageCommand;
        bool m_isAppBarEnabled;
        IPhoto^ m_photo;
        HubPhotoGroup^ m_pictureGroup;
        Windows::Foundation::EventRegistrationToken m_pictureGroupPropertyChangedToken;

        void NavigateToPictures(Platform::Object^ parameter);
        bool CanNavigateToPictures(Platform::Object^ parameter);
        void CropImage(Platform::Object^ parameter);
        bool CanCropImage(Platform::Object^ parameter);
        void RotateImage(Platform::Object^ parameter);
        bool CanCropOrRotateImage(Platform::Object^ parameter);
        void OnPictureGroupPropertyChanged(Platform::Object^ sender, Windows::UI::Xaml::Data::PropertyChangedEventArgs^ e);
    };
}

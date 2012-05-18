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
    class concurrency::cancellation_token_source;
    interface class IExceptionPolicy;
    ref class PhotoCache;
    ref class Photo;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class ImageBrowserViewModel sealed : public ViewModelBase
    {
    public:
        ImageBrowserViewModel(IExceptionPolicy^ exceptionPolicy);

        property Platform::Object^ MonthGroups
        {
            Platform::Object^ get();
        }

        property Platform::Object^ YearGroups
        {
            Platform::Object^ get();
        }

        property bool InProgress
        {
            bool get();
        }

        property Windows::UI::Xaml::Input::ICommand^ GroupCommand
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

        //       of the button to the SelectedItem property of the GridView.
        property Platform::Object^ SelectedItem
        {
            Platform::Object^ get();
            void set(Platform::Object^ value);
        }

        Photo^ GetPhotoForYearAndMonth(unsigned int year, unsigned int month);
        virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;        

    private:
        Platform::Collections::Vector<Platform::Object^>^ m_monthGroups;
        Platform::Collections::Vector<Platform::Object^>^ m_yearGroups;
        bool m_inProgress;
        Windows::UI::Xaml::Input::ICommand^ m_groupCommand;
        Windows::UI::Xaml::Input::ICommand^ m_cropImageCommand;
        Windows::UI::Xaml::Input::ICommand^ m_rotateImageCommand;

        concurrency::cancellation_token_source m_cancelTokenSource;
        PhotoCache^ m_photoCache;
        Platform::Object^ m_selectedItem;

        void NavigateToGroup(Platform::Object^ parameter);
        void CropImage(Platform::Object^ parameter);
        void RotateImage(Platform::Object^ parameter);
        bool CanEditImage(Platform::Object^ parameter);
    };
}

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
    interface class IRepository;
    interface class IPhoto;
    interface class IPhotoGroup;
    interface class IPhotoCache;
    interface class IYearGroup;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class ImageBrowserViewModel sealed : public ViewModelBase
    {
    public:
        ImageBrowserViewModel(IRepository^ repository, IExceptionPolicy^ exceptionPolicy);

        property Windows::Foundation::Collections::IObservableVector<IPhotoGroup^>^ MonthGroups
        {
            Windows::Foundation::Collections::IObservableVector<IPhotoGroup^>^ get();
        }

        property Windows::Foundation::Collections::IObservableVector<IYearGroup^>^ YearGroups
        {
            Windows::Foundation::Collections::IObservableVector<IYearGroup^>^ get();
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

        // These are really IPhoto^ but XAML cannot convert any underlying
        // real object (e.g. Photo) back to IPhoto which results in strange
        // side effects (such as the AppBar not working).
        property Platform::Object^ SelectedItem
        {
            Platform::Object^ get();
            void set(Platform::Object^ value);
        }

        IPhoto^ GetPhotoForYearAndMonth(unsigned int year, unsigned int month);
        virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;        

    internal:
        concurrency::task<void> QueryMonthGroupsAsync();
        concurrency::task<void> QueryYearGroupsAsync();

    private:
        Platform::Collections::Vector<IPhotoGroup^>^ m_monthGroups;
        Platform::Collections::Vector<IYearGroup^>^ m_yearGroups;
        Windows::UI::Xaml::Input::ICommand^ m_groupCommand;
        Windows::UI::Xaml::Input::ICommand^ m_cropImageCommand;
        Windows::UI::Xaml::Input::ICommand^ m_rotateImageCommand;
        bool m_inProgress;
        bool m_isAppBarEnabled;

        concurrency::cancellation_token_source m_monthCancelTokenSource;
        concurrency::cancellation_token_source m_yearCancelTokenSource;
        IPhotoCache^ m_photoCache;
        IPhoto^ m_photo;
        IRepository^ m_repository;

        void NavigateToGroup(Platform::Object^ parameter);
        void CropImage(Platform::Object^ parameter);
        void RotateImage(Platform::Object^ parameter);
        bool CanCropOrRotateImage(Platform::Object^ parameter);
    };
}

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
#include "PhotoPathComparer.h"

namespace Hilo
{
    interface class IPhoto;
    class ExceptionPolicy;
    class SinglePhotoQuery;
    class AllPhotosQuery;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class ImageViewModel sealed : public ViewModelBase
    {
    internal:
        ImageViewModel(std::shared_ptr<SinglePhotoQuery> singlePhotoQuery, std::shared_ptr<AllPhotosQuery> allPhotosQuery, std::shared_ptr<ExceptionPolicy> exceptionPolicy);
        
        virtual void SaveState(Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ stateMap) override;
        virtual void LoadState(Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ stateMap) override;
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
        virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

        concurrency::task<void> QueryPhotosAsync();
        concurrency::task<void> QuerySinglePhotoAsync();
        void Initialize(Platform::String^ filePath, Windows::Foundation::DateTime fileDate, Platform::String^ query);
        Platform::String^ GetStateFilePath();
        Platform::String^ GetStateQuery();
        Windows::Foundation::DateTime GetStateFileDate();

    public:
        virtual ~ImageViewModel();

        property Windows::Foundation::Collections::IObservableVector<IPhoto^>^ Photos
        { 
            Windows::Foundation::Collections::IObservableVector<IPhoto^>^ get();
        }

        property Platform::Object^ SelectedItem
        {
            Platform::Object^ get();
            void set(Platform::Object^ value);
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

    private:
        std::shared_ptr<SinglePhotoQuery> m_singlePhotoQuery;
        std::shared_ptr<AllPhotosQuery> m_allPhotosQuery;
        bool m_runningQuerySinglePhotoAsync;
        bool m_runningQueryPhotosAsync;
        Windows::Foundation::DateTime m_fileDate;
        Platform::String^ m_filePath;
        Platform::String^ m_query;
        IPhoto^ m_photo;
        Platform::Collections::Vector<IPhoto^, PhotoPathComparer>^ m_photos;
        concurrency::cancellation_token_source m_photosCts;
        concurrency::cancellation_token_source m_photoCts;
        Windows::UI::Xaml::Input::ICommand^ m_cropImageCommand;
        Windows::UI::Xaml::Input::ICommand^ m_rotateImageCommand;
        bool m_receivedChangeWhileRunning;

        void CropImage(Platform::Object^ parameter);
        void RotateImage(Platform::Object^ parameter);
        bool CanCropOrRotateImage(Platform::Object^ paratmer);
        void OnDataChanged();
        void ClearCachedData();
    };
}

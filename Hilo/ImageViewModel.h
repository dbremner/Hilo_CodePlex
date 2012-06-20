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
    interface class IExceptionPolicy;
    interface class IRepository;
    interface class IPhoto;
    ref class ImageNavigationData;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class ImageViewModel sealed : public ViewModelBase
    {
    public:
        ImageViewModel(IRepository^ repository, IExceptionPolicy^ exceptionPolicy);
        ~ImageViewModel();

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

        virtual void SaveState(Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ stateMap) override;
        virtual void LoadState(Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ stateMap) override;

        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
        virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

        void Initialize(ImageNavigationData^ imageNavigationData);
    
    internal:
        // Property retrievers largely here for testing.
        concurrency::task<void> QueryPhotosAsync();
        concurrency::task<void> QueryPhotoAsync();
        void SetPhoto(Windows::Foundation::Collections::IVectorView<Windows::Storage::BulkAccess::FileInformation^>^ files);

    private:
        IRepository^ m_repository;
        Windows::Foundation::EventRegistrationToken m_dataToken;
        bool m_runningGetPhotoAsync;
        bool m_runningGetPhotosAsync;
        Windows::Foundation::DateTime m_fileDate;
        Platform::String^ m_filePath;
        Platform::String^ m_query;
        IPhoto^ m_photo;
        Platform::Collections::Vector<IPhoto^, PhotoPathComparer>^ m_photos;
        concurrency::cancellation_token_source m_photosCts;
        concurrency::cancellation_token_source m_photoCts;
        Windows::UI::Xaml::Input::ICommand^ m_cropImageCommand;
        Windows::UI::Xaml::Input::ICommand^ m_rotateImageCommand;

        void CropImage(Platform::Object^ parameter);
        void RotateImage(Platform::Object^ parameter);
        
        void Initialize(Platform::String^ filePath, Windows::Foundation::DateTime fileDate, Platform::String^ query);

        void OnDataChanged();
        void ClearCachedData();
    };
}

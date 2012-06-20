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
    interface class IRepository;
    interface class IPhoto;
    ref class ImageNavigationData;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class RotateImageViewModel sealed : public ImageBase
    {
    public:
        RotateImageViewModel(IRepository^ repository, IExceptionPolicy^ exceptionPolicy);

        property Windows::UI::Xaml::Media::ImageSource^ Image
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

        property Windows::UI::Xaml::Thickness ImageMargin
        {
            Windows::UI::Xaml::Thickness get();
        }
        
        property bool InProgress { bool get(); }

        ///<summary>Returns the rotation angle for image display.</summary>
        property double RotationAngle 
        {
            double get();
            void set(double value);
        }

        virtual void LoadState(Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ stateMap) override;
        virtual void SaveState(Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ stateMap) override;
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

        void EndRotation();

    internal:
        void Initialize(ImageNavigationData^ navigationData);

    private:
        IRepository^ m_repository;
        IPhoto^ m_photo;
        Windows::UI::Xaml::Media::Imaging::BitmapImage^ m_image;
        Windows::Storage::Streams::IRandomAccessStream^ m_randomAccessStream;
        Windows::UI::Xaml::Input::ICommand^ m_rotateCommand;
        Windows::UI::Xaml::Input::ICommand^ m_saveCommand;
        Windows::UI::Xaml::Input::ICommand^ m_cancelCommand;
        Windows::UI::Xaml::Thickness m_imageMargin;
        bool m_inProgress;
        bool m_isSaving;
        double m_rotationAngle;
        bool m_isExifOrientation;
        unsigned int m_exifOrientation;
        ImageNavigationData^ m_navigationData;
        
        void ChangeInProgress(bool value);
        concurrency::task<IPhoto^> GetImagePhotoAsync();
        concurrency::task<void> RotateImageViewModel::RotateImageAsync(double angle);
        unsigned int ConvertExifOrientationToDegreesRotation(unsigned int exifOrientationFlag);
        unsigned int ConvertDegreesRotationToExifOrientation(unsigned int angle);

        void Rotate90(Platform::Object^ parameter);
        void SaveImage(Platform::Object^ parameter);
        void CancelRotate(Platform::Object^ parameter);
    };
}

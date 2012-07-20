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
    interface class IPhoto;
    class SinglePhotoQuery;
    class ExceptionPolicy;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class RotateImageViewModel sealed : public ImageBase
    {
    internal:
        RotateImageViewModel(std::shared_ptr<SinglePhotoQuery> query, std::shared_ptr<ExceptionPolicy> exceptionPolicy);

        virtual void LoadState(Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ stateMap) override;
        virtual void SaveState(Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ stateMap) override;
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

        void Initialize(Platform::String^ photoPath);
        void EndRotation();

    public:
        property IPhoto^ Photo
        {
            IPhoto^ get();
        }

        property Windows::UI::Xaml::Input::ICommand^ RotateCommand
        {
            Windows::UI::Xaml::Input::ICommand^ get();
        }

        property Windows::UI::Xaml::Input::ICommand^ ResumeRotateCommand
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

    private:

        struct ImageEncodingInformation
        {
            ImageEncodingInformation() : usesExifOrientation(false), exifOrientation(0){}

            Windows::Graphics::Imaging::BitmapDecoder^ decoder;    
            bool usesExifOrientation;
            unsigned short exifOrientation;
        };

        std::shared_ptr<SinglePhotoQuery> m_query;
        bool m_getPhotoAsyncIsRunning;

        Windows::UI::Xaml::Input::ICommand^ m_rotateCommand;
        Windows::UI::Xaml::Input::ICommand^ m_resumeRotateCommand;
        Windows::UI::Xaml::Input::ICommand^ m_saveCommand;
        Windows::UI::Xaml::Input::ICommand^ m_cancelCommand;
        Windows::UI::Xaml::Thickness m_imageMargin;
        bool m_inProgress;
        bool m_isSaving;
        double m_rotationAngle;
        Platform::String^ m_photoPath;

        void ChangeInProgress(bool value);
        concurrency::task<IPhoto^> GetImagePhotoAsync();
        concurrency::task<Windows::Storage::Streams::IRandomAccessStream^> RotateImageViewModel::RotateImageAsync(Windows::Storage::Streams::IRandomAccessStream^ sourceStream, double angle);       
        unsigned int CheckRotationAngle(unsigned int angle);
        void Rotate90(Platform::Object^ parameter);
        void Unsnap(Platform::Object^ parameter);
        void SaveImage(Platform::Object^ parameter);
        void CancelRotate(Platform::Object^ parameter);


        concurrency::task<ImageEncodingInformation> GetDecoderInfo(Windows::Storage::Streams::IRandomAccessStream^ source, concurrency::task_continuation_context backgroundContext);
        concurrency::task<Windows::Storage::Streams::IRandomAccessStream^> EncodeRotateImageToStream(
            ImageEncodingInformation decoderInfo, 
            double rotationAngle, 
            concurrency::task_continuation_context backgroundContext);
        concurrency::task<Windows::Graphics::Imaging::BitmapEncoder^> RotateImageViewModel::SetEncodingRotation(
            Windows::Graphics::Imaging::BitmapEncoder^ encoder, 
            std::shared_ptr<ImageEncodingInformation> decoderInfo, 
            double rotationAngle, 
            concurrency::task_continuation_context backgroundContext);
    };
}

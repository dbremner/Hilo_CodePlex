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
    interface class IPhoto;
    interface class IRepository;
    ref class ImageNavigationData;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class CropImageViewModel sealed : public ImageBase
    {
    public:
        CropImageViewModel(IRepository^ repository, IExceptionPolicy^ exceptionPolicy);

        property Windows::UI::Xaml::Media::ImageSource^ Image
        {
            Windows::UI::Xaml::Media::ImageSource^ get();
        }

        property Windows::UI::Xaml::Input::ICommand^ SaveCommand
        {
            Windows::UI::Xaml::Input::ICommand^ get();
        }

        property Windows::UI::Xaml::Input::ICommand^ CancelCommand
        {
            Windows::UI::Xaml::Input::ICommand^ get();
        }

        property bool InProgress { bool get(); }
        property double CropOverlayLeft { double get(); }
        property double CropOverlayTop { double get(); }
        property double CropOverlayHeight { double get(); }
        property double CropOverlayWidth { double get();} 
        property bool IsCropOverlayVisible { bool get(); }

        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

        void CalculateInitialCropOverlayPosition(Windows::UI::Xaml::Media::GeneralTransform^ transform, float width, float height);
        void UpdateCropOverlayPostion(Windows::UI::Xaml::Controls::Primitives::Thumb^ thumb, double verticalChange, double horizontalChange, double minWidth, double minHeight);
        Windows::Foundation::IAsyncAction^ CropImageAsync(double actualWidth);

    internal:
        void Initialize(ImageNavigationData^ navigationData);

    private:
        IRepository^ m_repository;
        IPhoto^ m_photo;
        Windows::UI::Xaml::Media::Imaging::WriteableBitmap^ m_image;
        Windows::Storage::Streams::IRandomAccessStream^ m_imageStream;
        Windows::UI::Xaml::Input::ICommand^ m_saveCommand;
        Windows::UI::Xaml::Input::ICommand^ m_cancelCommand;
        bool m_inProgress;
        bool m_isCropOverlayVisible;
        bool m_isSaving;

        double m_left;
        double m_top;
        double m_right;
        double m_bottom;

        double m_cropOverlayLeft;
        double m_cropOverlayTop;
        double m_cropOverlayHeight;
        double m_cropOverlayWidth;
        double m_actualHeight;
        double m_actualWidth;       

        unsigned int m_cropX;
        unsigned int m_cropY;

        void ChangeInProgress(bool value);
        concurrency::task<void> GetImageAsync();
        concurrency::task<void> EncodeImageAsync();
        byte* GetPointerToPixelData(Windows::Storage::Streams::IBuffer^ buffer);

        // Member functions that implement Commands
        void SaveImage(Platform::Object^ parameter);
        void CancelCrop(Platform::Object^ parameter);
        concurrency::task<void> CropImageAsyncImpl(double actualWidth);
    };
}


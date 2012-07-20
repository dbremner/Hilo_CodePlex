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
    class ImageNavigationData;
    class SinglePhotoQuery;
    class ExceptionPolicy;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class CropImageViewModel sealed : public ImageBase
    {
    internal:
        CropImageViewModel(std::shared_ptr<SinglePhotoQuery> query, std::shared_ptr<ExceptionPolicy> exceptionPolicy);

        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

        void Initialize(Platform::String^ photoPath);
        void CalculateInitialCropOverlayPosition(Windows::UI::Xaml::Media::GeneralTransform^ transform, float width, float height);
        void UpdateCropOverlayPostion(Windows::UI::Xaml::Controls::Primitives::Thumb^ thumb, double verticalChange, double horizontalChange, double minWidth, double minHeight);
        concurrency::task<void> CropImageAsync(double actualWidth);

    public:
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

        property Windows::UI::Xaml::Input::ICommand^ ResumeCropCommand
        {
            Windows::UI::Xaml::Input::ICommand^ get();
        }

        property bool InProgress { bool get(); }
        property double CropOverlayLeft { double get(); }
        property double CropOverlayTop { double get(); }
        property double CropOverlayHeight { double get(); }
        property double CropOverlayWidth { double get();} 
        property bool IsCropOverlayVisible { bool get(); }

    private:
        std::shared_ptr<SinglePhotoQuery> m_query;
        Windows::UI::Xaml::Media::Imaging::WriteableBitmap^ m_image;
        Windows::UI::Xaml::Input::ICommand^ m_resumeCropCommand;
        Windows::UI::Xaml::Input::ICommand^ m_saveCommand;
        Windows::UI::Xaml::Input::ICommand^ m_cancelCommand;
        bool m_inProgress;
        bool m_isCropOverlayVisible;
        bool m_isSaving;
        Platform::String^ m_photoPath;

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
        concurrency::task<IPhoto^> GetImagePhotoAsync();
        concurrency::task<Windows::Storage::Streams::IRandomAccessStream^> EncodeImageAsync(Windows::Storage::Streams::IRandomAccessStream^ sourceStream);
        byte* GetPointerToPixelData(Windows::Storage::Streams::IBuffer^ buffer);
        void DoCrop(uint32_t xOffset, uint32_t yOffset, uint32_t newHeight, uint32_t newWidth, uint32_t oldWidth, byte *pSrcPixels, byte *pDestPixels);

        // Member functions that implement Commands
        void SaveImage(Platform::Object^ parameter);
        void CancelCrop(Platform::Object^ parameter);
        void Unsnap(Platform::Object^ parameter);
    };
}

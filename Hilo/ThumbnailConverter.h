//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

namespace Hilo
{
    namespace Converters
    {
        public ref class ThumbnailConverter sealed : public Windows::UI::Xaml::Data::IValueConverter
        {
        public:
            virtual Platform::Object^ Convert(Platform::Object^ value,
                Windows::UI::Xaml::Interop::TypeName targetType,
                Platform::Object^ parameter,
                Platform::String^ culture)
            {
                if (value != nullptr)
                {
                    Windows::Storage::Streams::IRandomAccessStream^ thumbnailStream = (Windows::Storage::Streams::IRandomAccessStream^) value;
                    Windows::UI::Xaml::Media::Imaging::BitmapImage^ bmp = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage();
                    bmp->SetSource(thumbnailStream);

                    return bmp;
                }
                return Windows::UI::Xaml::DependencyProperty::UnsetValue;
            }

            virtual Platform::Object^ ConvertBack(Platform::Object^ value,
                Windows::UI::Xaml::Interop::TypeName targetType,
                Platform::Object^ parameter,
                Platform::String^ culture)
            {
                throw ref new Platform::NotImplementedException();
            }
        };
    }
}
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
    public ref class ThumbnailConverter sealed : public Windows::UI::Xaml::Data::IValueConverter
    {
    public:
        virtual Platform::Object^ Convert(Platform::Object^ value,
            Windows::UI::Xaml::Interop::TypeName targetType,
            Platform::Object^ parameter,
            Platform::String^ culture);

        virtual Platform::Object^ ConvertBack(Platform::Object^ value,
            Windows::UI::Xaml::Interop::TypeName targetType,
            Platform::Object^ parameter,
            Platform::String^ culture);
    };
}

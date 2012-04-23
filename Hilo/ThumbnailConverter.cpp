//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ThumbnailConverter.h"

using namespace Hilo::Converters;

using namespace Platform;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media::Imaging;

Object^ ThumbnailConverter::Convert(Object^ value, TypeName targetType, Object^ parameter, String^ culture)
{
    if (value != nullptr)
    {
        IRandomAccessStream^ thumbnailStream = (IRandomAccessStream^) value;
        BitmapImage^ bmp = ref new BitmapImage();
        bmp->SetSource(thumbnailStream);

        return bmp;
    }
    return Windows::UI::Xaml::DependencyProperty::UnsetValue;
}

Object^ ThumbnailConverter::ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ culture)
{
    throw ref new Platform::NotImplementedException();
}

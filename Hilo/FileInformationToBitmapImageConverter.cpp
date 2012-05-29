//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "FileInformationToBitmapImageConverter.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media::Imaging;

Object^ FileInformationToBitmapImageConverter::Convert(Object^ value, TypeName targetType, Object^ parameter, String^ culture)
{
    BitmapImage^ image = nullptr;

    if (value != nullptr)
    {
        FileInformation^ file = safe_cast<FileInformation^>(value);
        image = ref new BitmapImage();

        auto fileOpenTask = task<IRandomAccessStreamWithContentType^>(file->OpenReadAsync());
        fileOpenTask.then([image](IRandomAccessStreamWithContentType^ imageData)
        {
            image->SetSource(imageData);
            return image;
        }, task_continuation_context::use_current());
    }
    return image;
}

Object^ FileInformationToBitmapImageConverter::ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ culture)
{
    throw ref new Platform::NotImplementedException();
}

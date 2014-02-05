// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "StubPhotoImage.h"
#include "..\Hilo\IPhotoGroup.h"
#include "StubPhotoGroup.h"

using namespace concurrency;
using namespace Hilo;
using namespace HiloTests;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Streams;
using namespace Windows::System::UserProfile;
using namespace Windows::UI::Xaml::Media::Imaging;

StubPhotoImage::StubPhotoImage()
{
    m_name = "Stub Photo";
}

StubPhotoImage::StubPhotoImage(String^ path, String^ name) : m_path(path), m_name(name)
{
}

String^ StubPhotoImage::Name::get()
{
    return m_name;
}

String^ StubPhotoImage::Path::get()
{
    return m_path;
}

void StubPhotoImage::Path::set(String^ value)
{
    m_path = value;
}

String^ StubPhotoImage::FormattedPath::get()
{
    std::wstring pathAndFileName = m_name->Data();
    std::basic_string<char>::size_type index;
    index = pathAndFileName.rfind('\\');
    std::wstring path = pathAndFileName.substr(0, index);
    return ref new String(path.c_str());
}

String^ StubPhotoImage::FileType::get()
{
    return m_fileType;
}

void StubPhotoImage::SetDateTaken(DateTime value)
{
    m_dateTaken = value;
}

String^ StubPhotoImage::FormattedDateTaken::get()
{
    DateTimeFormatter dtf("shortdate", GlobalizationPreferences::Languages);
    return dtf.Format(m_dateTaken);
}

String^ StubPhotoImage::FormattedTimeTaken::get()
{
    DateTimeFormatter dtf("shorttime", GlobalizationPreferences::Languages);
    return dtf.Format(m_dateTaken);
}

BitmapImage^ StubPhotoImage::Thumbnail::get()
{   
    return m_thumbnail;
}

BitmapImage^ StubPhotoImage::Image::get()
{
    return m_image;
}

bool StubPhotoImage::IsInvalidThumbnail::get()
{
    return m_isInvalidThumbnail;
}

String^ StubPhotoImage::Resolution::get()
{
    return m_resolution;
}

uint64 StubPhotoImage::FileSize::get()
{
    return m_fileSize;
}

String^ StubPhotoImage::DisplayType::get()
{
    return m_displayType;
}

void StubPhotoImage::ClearImageData()
{
    // not implemented
}

Windows::Foundation::IAsyncAction^ StubPhotoImage::InitializeAsync()
{
    // not implemented
    return create_async([]{});
}

IAsyncOperation<IRandomAccessStreamWithContentType^>^ StubPhotoImage::OpenReadAsync()
{
    return create_async([this]
    {
       return m_randomAccessStream;
    });
}


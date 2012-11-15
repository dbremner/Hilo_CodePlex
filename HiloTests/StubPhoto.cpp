// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "StubPhoto.h"
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

StubPhoto::StubPhoto()
{
    m_name = "Stub Photo";
}

IPhotoGroup^ StubPhoto::Group::get()
{
    return ref new StubPhotoGroup("");
}

String^ StubPhoto::Name::get()
{
    return m_name;
}

String^ StubPhoto::Path::get()
{
    return m_path;
}

void StubPhoto::Path::set(String^ value)
{
    m_path = value;
}

String^ StubPhoto::FormattedPath::get()
{
    std::wstring pathAndFileName = m_name->Data();
    std::basic_string<char>::size_type index;
    index = pathAndFileName.rfind('\\');
    std::wstring path = pathAndFileName.substr(0, index);
    return ref new String(path.c_str());
}

String^ StubPhoto::FileType::get()
{
    return m_fileType;
}

DateTime StubPhoto::DateTaken::get()
{
    return m_dateTaken;
}

void StubPhoto::DateTaken::set(DateTime value)
{
    m_dateTaken = value;
}

String^ StubPhoto::FormattedDateTaken::get()
{
    DateTimeFormatter dtf("shortdate", GlobalizationPreferences::Languages);
    return dtf.Format(DateTaken);
}

String^ StubPhoto::FormattedTimeTaken::get()
{
    DateTimeFormatter dtf("shorttime", GlobalizationPreferences::Languages);
    return dtf.Format(DateTaken);
}

BitmapImage^ StubPhoto::Thumbnail::get()
{   
    return m_thumbnail;
}

BitmapImage^ StubPhoto::Image::get()
{
    return m_image;
}

bool StubPhoto::IsInvalidThumbnail::get()
{
    return m_isInvalidThumbnail;
}

String^ StubPhoto::Resolution::get()
{
    return m_resolution;
}

uint64 StubPhoto::FileSize::get()
{
    return m_fileSize;
}

String^ StubPhoto::DisplayType::get()
{
    return m_displayType;
}

void StubPhoto::ClearImageData()
{
    // not implemented
}

IAsyncOperation<IRandomAccessStreamWithContentType^>^ StubPhoto::OpenReadAsync()
{
    return create_async([this]
    {
       return m_randomAccessStream;
    });
}

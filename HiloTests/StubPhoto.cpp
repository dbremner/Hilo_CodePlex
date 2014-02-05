// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "StubPhoto.h"
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

void StubPhoto::SetDateTaken(DateTime value)
{
    m_dateTaken = value;
}

BitmapImage^ StubPhoto::Thumbnail::get()
{   
    return m_thumbnail;
}

bool StubPhoto::IsInvalidThumbnail::get()
{
    return m_isInvalidThumbnail;
}

Windows::Foundation::IAsyncOperation<Windows::Foundation::DateTime>^ StubPhoto::GetDateTakenAsync()
{
    assert(false); // not implemented
    return create_async([]{ DateTime dt = {0ll}; return dt; });
}

IPhotoImage^ StubPhoto::GetPhotoImage()
{
    return ref new StubPhotoImage(m_path, m_name);
}

int StubPhoto::RowSpan::get()
{
    return m_rowSpan;
}

void StubPhoto::RowSpan::set(int value)
{
    m_rowSpan = value;
}

int StubPhoto::ColumnSpan::get()
{
    return m_columnSpan;
}

void StubPhoto::ColumnSpan::set(int value)
{
    m_columnSpan = value;
}



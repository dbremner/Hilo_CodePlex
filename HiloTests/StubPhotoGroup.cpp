// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "StubPhotoGroup.h"
#include "..\Hilo\IPhoto.h"

using namespace Hilo;
using namespace HiloTests;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;

StubPhotoGroup::StubPhotoGroup(String^ name)
{
}

StubPhotoGroup::operator IStorageFolder^()
{
    return nullptr;
}

String^ StubPhotoGroup::Title::get()
{ 
    return "";
}

IObservableVector<IPhoto^>^ StubPhotoGroup::Items::get()
{
    return ref new Vector<IPhoto^>();
}
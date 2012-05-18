//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "StubPhotoGroup.h"

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

IObservableVector<Platform::Object^>^ StubPhotoGroup::Items::get()
{
    return ref new Vector<Object^>();
}
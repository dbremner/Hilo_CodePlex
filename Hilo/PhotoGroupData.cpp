//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "PhotoGroupData.h"
#include "IPhoto.h"

using namespace Hilo;
using namespace Windows::Foundation::Collections;

PhotoGroupData::PhotoGroupData(Windows::Foundation::Collections::IVectorView<IPhoto^>^ photos, unsigned int size) : m_photos(photos), m_size(size)
{
}

IVectorView<IPhoto^>^ PhotoGroupData::Photos::get()
{
    return m_photos;
}

unsigned int PhotoGroupData::Size::get()
{
    return m_size;
}

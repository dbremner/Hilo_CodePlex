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

PhotoGroupData::PhotoGroupData()
{
}

PhotoGroupData::PhotoGroupData(IVectorView<IPhoto^>^ photos, unsigned int size) : m_photos(photos), m_size(size)
{
}

PhotoGroupData::PhotoGroupData(const PhotoGroupData& data)
{
    m_photos = data.m_photos;
    m_size = data.m_size;
}

IVectorView<IPhoto^>^ PhotoGroupData::GetPhotos() const
{
    return m_photos;
}

unsigned int PhotoGroupData::GetSize() const
{
    return m_size;
}

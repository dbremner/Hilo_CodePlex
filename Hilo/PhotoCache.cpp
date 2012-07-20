//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "PhotoCache.h"
#include "IPhoto.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::Globalization;

void PhotoCache::InsertPhoto(IPhoto^ photo)
{
    Calendar cal;
    auto dateTaken = photo->DateTaken;
    cal.SetDateTime(dateTaken);
    auto &yearPair = m_photoCache[cal.Year];
    yearPair[cal.Month] = WeakReference(photo);
}

IPhoto^ PhotoCache::GetForYearAndMonth(int year, int month)
{
    auto &yearMap = m_photoCache[year];
    auto &weakRef = yearMap[month];

    if (nullptr != weakRef)
    {
        return weakRef.Resolve<IPhoto>();
    }
    return nullptr;
}

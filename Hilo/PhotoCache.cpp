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
#include "Photo.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Globalization;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::FileProperties;

PhotoCache::PhotoCache()
{
}

void PhotoCache::InsertPhoto(Photo^ photo)
{
    FileInformation^ fileInfo = photo;
    Calendar cal;
    auto dateTaken = DateForFileInformation(fileInfo);
    cal.FromDateTime(dateTaken);
    auto &yearPair = m_photoCache[cal.Year];
    yearPair[cal.Month] = WeakReference(photo);
}

Photo^ PhotoCache::GetForYearAndMonth(int year, int month)
{
    auto &yearMap = m_photoCache[year];
    auto &weakRef = yearMap[month];

    if (nullptr != weakRef)
    {
        return weakRef.Resolve<Photo>();
    }
    return nullptr;
}

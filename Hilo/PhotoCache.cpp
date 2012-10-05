#include "pch.h"
#include "PhotoCache.h"
#include "IPhoto.h"
#include "CalendarExtensions.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::Globalization;

void PhotoCache::Clear()
{
    m_photoCache.clear();
}

void PhotoCache::InsertPhoto(IPhoto^ photo)
{
    int year, month;
    auto monthDate = photo->DateTaken;

    CalendarExtensions::WriteLocalizedYearAndMonth(monthDate, year, month);
    auto &yearPair = m_photoCache[year];
    yearPair[month] = WeakReference(photo);
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

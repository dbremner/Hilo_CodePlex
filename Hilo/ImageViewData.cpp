//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ImageViewData.h"
#include "Photo.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Globalization;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::System::UserProfile;

ImageViewData::ImageViewData(Hilo::Photo^ photo)
{
    FileInformation^ fileInfo = photo;
    m_fileDate = DateForFileInformation(fileInfo);
    m_filePath = fileInfo->Path;
}

void ImageViewData::Foo(IStorageItemInformation^ s, Object^ e)
{
}

DateTime ImageViewData::FileDate::get()
{
    return m_fileDate;
}

String^ ImageViewData::FilePath::get()
{
    return m_filePath;
}

String^ ImageViewData::DateQuery::get()
{
    if (nullptr == m_dateQuery)
    {
        Calendar cal;
        cal.FromDateTime(m_fileDate);

        int lastDay = cal.LastDayInThisMonth;
        int firstDay = cal.FirstDayInThisMonth;
        DateTimeFormatter dtf("shortdate", GlobalizationPreferences::Languages);
        cal.Day = firstDay;
        String^ firstDate = dtf.Format(cal.ToDateTime());

        cal.Day = lastDay;
        String^ lastDate = dtf.Format(cal.ToDateTime());
        
        std::wstringstream dateRange;
        dateRange << L"System.ItemDate:" << firstDate->Data() << ".." << lastDate->Data();
        m_dateQuery = ref new String(dateRange.str().c_str());
    }
    return m_dateQuery;
}

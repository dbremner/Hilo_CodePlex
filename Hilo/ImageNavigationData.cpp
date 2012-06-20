//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ImageNavigationData.h"
#include "IPhoto.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Globalization;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::System::UserProfile;

ImageNavigationData::ImageNavigationData(Hilo::IPhoto^ photo)
{
    m_fileDate = photo->DateTaken;
    m_filePath = photo->Path;
}

ImageNavigationData::ImageNavigationData(String^ serializedData)
{
    std::wstring data(serializedData->Data());

    auto index = data.find('|');
    assert(index > 0);

    auto path = data.substr(0, index);
    auto date = data.substr(index+1, data.length());

    DateTime fileDate;
    fileDate.UniversalTime = _wtoi64(date.c_str());

    m_filePath = ref new String(path.c_str());
    m_fileDate = fileDate;
}

DateTime ImageNavigationData::FileDate::get()
{
    return m_fileDate;
}

String^ ImageNavigationData::FilePath::get()
{
    return m_filePath;
}

String^ ImageNavigationData::DateQuery::get()
{
    if (nullptr == m_dateQuery)
    {
        Calendar cal;
        cal.SetDateTime(m_fileDate);

        int lastDay = cal.LastDayInThisMonth;
        int firstDay = cal.FirstDayInThisMonth;
        DateTimeFormatter dtf("shortdate", GlobalizationPreferences::Languages);
        cal.Day = firstDay;
        String^ firstDate = dtf.Format(cal.GetDateTime());

        cal.Day = lastDay;
        String^ lastDate = dtf.Format(cal.GetDateTime());

        std::wstringstream dateRange;
        dateRange << L"System.ItemDate:" << firstDate->Data() << ".." << lastDate->Data();
        m_dateQuery = ref new String(dateRange.str().c_str());
    }
    return m_dateQuery;
}

String^ ImageNavigationData::SerializeToString()
{
    std::wstringstream stringStream;

    stringStream << m_filePath->Data() << L"|" << m_fileDate.UniversalTime ;
    
    return ref new String(stringStream.str().c_str());
}

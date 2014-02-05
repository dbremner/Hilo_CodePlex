// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "ImageNavigationData.h"
#include "IPhoto.h"
#include "CalendarExtensions.h"


using namespace Hilo;
using namespace Platform;
using namespace std;
using namespace Windows::Foundation;


ImageNavigationData::ImageNavigationData(String^ path, DateTime monthGroupDate)
{
    assert(path != nullptr);

    m_monthGroupDate = monthGroupDate;
    m_filePath = path;
}

ImageNavigationData::ImageNavigationData(String^ serializedData)
{
    wstring data(serializedData->Data());
    auto index = data.find('|');
    assert(index > 0);

    auto path = data.substr(0, index);
    auto date = data.substr(index+1, data.length());
    DateTime monthGroupDate;
    monthGroupDate.UniversalTime = _wtoi64(date.c_str());

    m_filePath = ref new String(path.c_str());
    m_monthGroupDate = monthGroupDate;
}

DateTime ImageNavigationData::GetMonthGroupDate() const
{
    return m_monthGroupDate;
}

String^ ImageNavigationData::GetFilePath() const
{
    return m_filePath;
}

String^ ImageNavigationData::GetDateQuery()
{
    if (nullptr == m_dateQuery)
    {
        m_dateQuery = CalendarExtensions::CreateMonthRangeFromDate(m_monthGroupDate);
    }
    return m_dateQuery;
}

String^ ImageNavigationData::SerializeToString()
{
    wstringstream stringStream;
    stringStream << m_filePath->Data() << L"|" << m_monthGroupDate.UniversalTime ;
    return ref new String(stringStream.str().c_str());
}

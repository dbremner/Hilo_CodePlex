//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "MonthBlock.h"
#include "ExceptionPolicy.h"
#include "IYearGroup.h"
#include "IResourceLoader.h"
#include "TaskExceptionsExtensions.h"
#include "MonthBlockQuery.h"

using namespace concurrency;
using namespace std;
using namespace Hilo;
using namespace Platform;
using namespace Windows::Globalization;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::System::UserProfile;

MonthBlock::MonthBlock(IYearGroup^ yearGroup, unsigned int month, IResourceLoader^ resourceLoader, shared_ptr<MonthBlockQuery> query, shared_ptr<ExceptionPolicy> exceptionPolicy) : m_weakYearGroup(yearGroup), m_month(month), m_resourceLoader(resourceLoader), m_query(query), m_exceptionPolicy(exceptionPolicy), m_runOperation(false)
{
}

unsigned int MonthBlock::Month::get()
{
    return m_month;
}

String^ MonthBlock::Name::get()
{
    wstringstream dateRange;
    dateRange << m_month;
    return m_resourceLoader->GetString(ref new String(dateRange.str().c_str()));
}

IYearGroup^ MonthBlock::Group::get()
{
    return m_weakYearGroup.Resolve<IYearGroup>();
}

bool MonthBlock::HasPhotos::get()
{
    if (!m_runOperation && !m_runningOperation)
    {
        QueryPhotoCount().then([this]
        {
            OnPropertyChanged("HasPhotos");
        }).then(ObserveException<void>(m_exceptionPolicy));
    }
    return (m_count > 0);
}

task<void> MonthBlock::QueryPhotoCount()
{
    m_runningOperation = true;
    auto dateRangeQuery = BuildDateQuery();
    auto t = m_query->GetPhotoCountWithDateRangeQueryAsync(dateRangeQuery, cancellation_token::none());
    return t.then([this](unsigned int count)
    {
        m_count = count;
        m_runOperation = true;
        m_runningOperation = true;
    });
}

String^ MonthBlock::BuildDateQuery()
{
    Calendar cal;
    cal.Year = Group->Year;
    cal.Month = m_month;
    int firstDay = cal.FirstDayInThisMonth;
    int lastDay = cal.LastDayInThisMonth;
    DateTimeFormatter dtf("shortdate", GlobalizationPreferences::Languages);
    cal.Day = firstDay;
    String^ firstDate = dtf.Format(cal.GetDateTime());
    cal.Day = lastDay;
    String^ lastDate = dtf.Format(cal.GetDateTime());
    wstringstream dateRange;
    dateRange << L"System.ItemDate:" 
              << firstDate->Data() 
              << L".." 
              << lastDate->Data();
    return ref new String(dateRange.str().c_str());
}
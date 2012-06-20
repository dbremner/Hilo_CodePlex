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
#include "IExceptionPolicy.h"
#include "IMonthBlock.h"
#include "IYearGroup.h"
#include "IQueryOperation.h"
#include "IRepository.h"
#include "IResourceLoader.h"
#include "TaskExceptionsExtensions.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Globalization;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::System::UserProfile;
using namespace Windows::UI::Xaml::Media::Imaging;


MonthBlock::MonthBlock(IYearGroup^ yearGroup, unsigned int month, IResourceLoader^ resourceLoader, IRepository^ repository, IQueryOperation^ queryOperation, IExceptionPolicy^ exceptionPolicy) : m_weakYearGroup(yearGroup), m_month(month), m_resourceLoader(resourceLoader), m_repository(repository), m_queryOperation(queryOperation), m_exceptionPolicy(exceptionPolicy), m_runOperation(false)
{
}

unsigned int MonthBlock::Month::get()
{
    return m_month;
}

String^ MonthBlock::Name::get()
{
    std::wstringstream dateRange;
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
    m_queryOperation->Query = BuildDateQuery();
    auto t = create_task(m_repository->GetPhotoCountForQueryOperationAsync(m_queryOperation));
    return t.then([this](unsigned int count)
    {
        auto s = BuildDateQuery();
        m_count = count;
        m_runOperation = true;
        m_runningOperation = true;
    }, task_continuation_context::use_arbitrary());
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
    std::wstringstream dateRange;
    dateRange << L"System.ItemDate:" << firstDate->Data() << ".." << lastDate->Data();
    return ref new String(dateRange.str().c_str());
}
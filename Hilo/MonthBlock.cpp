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
#include "YearGroup.h"
#include "PhotoReader.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Windows::ApplicationModel::Resources;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Globalization;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Search;
using namespace Windows::UI::Xaml::Media::Imaging;

MonthBlock::MonthBlock(YearGroup^ yearGroup, unsigned int month) : m_weakYearGroup(yearGroup), m_month(month), m_count(0)
{
}

unsigned int MonthBlock::Month::get()
{
    return m_month;
}

String^ MonthBlock::Name::get()
{
    auto loader = ref new ResourceLoader();
    std::wstringstream dateRange;
    dateRange << m_month;
    return loader->GetString(ref new String(dateRange.str().c_str()));
}

YearGroup^ MonthBlock::Group::get()
{
    return m_weakYearGroup.Resolve<YearGroup>();
}

String^ MonthBlock::BuildDateQuery()
{
    //std::wstring wname = folderName->Data() + 1;
    int year = Group->Year; //std::stoi(wname);
    Calendar cal;
    cal.Year = year;
    cal.Month = m_month;
    int lastDay = cal.LastDayInThisMonth;
    int firstDay = cal.FirstDayInThisMonth;
    std::wstringstream dateRange;
    dateRange << L"System.ItemDate:" << m_month << L"/" << firstDay << L"/" << year << ".." << m_month << L"/" << lastDay << L"/" << year;
    return ref new String(dateRange.str().c_str());
}

bool MonthBlock::Enabled::get()
{
    if (!m_determinedEnabled)
    {
        IStorageFolder^ folder = Group;
        String^ dateRangeQuery = BuildDateQuery();
        IStorageFolderQueryOperations^ query = dynamic_cast<IStorageFolderQueryOperations^>(folder);
        PhotoReader reader;
        task<IVectorView<FileInformation^>^> monthTask = reader.GetPhotosAsync(query, dateRangeQuery);
        monthTask.then([this](IVectorView<FileInformation^>^ files)
        {
            m_count = files->Size;
            m_determinedEnabled = true;
            OnPropertyChanged("Enabled");
        }, task_continuation_context::use_current());
    }
    return (m_count > 0);
}
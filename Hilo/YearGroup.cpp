//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "YearGroup.h"
#include "MonthBlock.h"
#include "ExceptionPolicy.h"
#include "LocalResourceLoader.h"
#include "MonthBlockQuery.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace std;
using namespace Windows::Foundation::Collections;

const std::array<int, 12> months = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

YearGroup::YearGroup(String^ name, shared_ptr<MonthBlockQuery> query, shared_ptr<ExceptionPolicy> exceptionPolicy) : m_name(name), m_query(query), m_exceptionPolicy(exceptionPolicy)
{
    wstring wname = name->Data() + 1;
    m_year = std::stoi(wname);
}

IObservableVector<IMonthBlock^>^ YearGroup::Items::get()
{
    if (nullptr == m_months)
    {
        vector<IMonthBlock^> monthBlocks;
        for (auto month : months)
        {
            auto monthBlock = ref new MonthBlock(this, month, ref new LocalResourceLoader(), m_query, m_exceptionPolicy);
            monthBlocks.push_back(monthBlock);
        }
        m_months = ref new Vector<IMonthBlock^>(monthBlocks);
    }
    return m_months;
}

String^ YearGroup::Title::get()
{
    return m_name;
}

unsigned int YearGroup::Year::get()
{
    return m_year;
}
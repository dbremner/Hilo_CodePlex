//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "Photo.h"
#include "YearGroup.h"
#include "MonthBlock.h"
#include "IExceptionPolicy.h"
#include "IRepository.h"
#include "IQueryOperation.h"
#include "LocalResourceLoader.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Globalization;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Search;
using namespace Windows::System::UserProfile;
using namespace Windows::UI::Core;

const std::array<int, 12> items = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };


YearGroup::YearGroup(Platform::String^ name, IRepository^ repository, IQueryOperation^ operation, IExceptionPolicy^ exceptionPolicy) : m_name(name), m_repository(repository), m_operation(operation), m_exceptionPolicy(exceptionPolicy)
{
    std::wstring wname = name->Data() + 1;
    m_year = std::stoi(wname);
}

IObservableVector<IMonthBlock^>^ YearGroup::Items::get()
{
    if (nullptr == m_months)
    {
        std::vector<IMonthBlock^> months;
        for (auto month : items)
        {
            auto monthBlock = ref new MonthBlock(this, month, ref new LocalResourceLoader(), m_repository, m_operation, m_exceptionPolicy);
            months.push_back(monthBlock);
        }
        m_months = ref new Vector<IMonthBlock^>(months);
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
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
#include "PhotoReader.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Search;
using namespace Windows::UI::Core;

const std::array<int, 12> items = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };


YearGroup::YearGroup(IStorageFolder^ storagefolder) : m_storageFolder(storagefolder)
{
    std::wstring wname = storagefolder->Name->Data() + 1;
    m_year = std::stoi(wname);
}


YearGroup::operator IStorageFolder^()
{
    return m_storageFolder;
}

IObservableVector<Object^>^ YearGroup::Items::get()
{
    if (nullptr == m_months)
    {
        std::vector<Object^> months;
        std::for_each(begin(items), end(items), [this, &months](int month)
        {
            auto monthBlock = ref new MonthBlock(this, month);
            months.push_back(monthBlock);
        });
        m_months = ref new Vector<Object^>(months);
    }
    return m_months;
}

String^ YearGroup::Title::get()
{
    return m_storageFolder->Name;
}

unsigned int YearGroup::Year::get()
{
    return m_year;
}
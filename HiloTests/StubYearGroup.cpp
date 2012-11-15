// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "StubYearGroup.h"
#include "..\Hilo\IMonthBlock.h"
#include "..\Hilo\CalendarExtensions.h"

using namespace HiloTests;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

StubYearGroup::StubYearGroup(DateTime yearDate) : m_yearDate(yearDate)
{
}

String^ StubYearGroup::Title::get()
{
    return CalendarExtensions::GetLocalizedYear(m_yearDate);
}

int StubYearGroup::Year::get()
{
    return CalendarExtensions::GetNumericYear(m_yearDate);
}

IObservableVector<IMonthBlock^>^ StubYearGroup::Items::get()
{
    return ref new Vector<IMonthBlock^>();
}
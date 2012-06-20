//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "StubYearGroup.h"
#include "..\Hilo\IMonthBlock.h"

using namespace HiloTests;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;

StubYearGroup::StubYearGroup(String^ title, unsigned int year) : m_title(title), m_year(year)
{
}

String^ StubYearGroup::Title::get()
{
    return m_title;
}

unsigned int StubYearGroup::Year::get()
{
    return m_year;
}

IObservableVector<IMonthBlock^>^ StubYearGroup::Items::get()
{
    return ref new Vector<IMonthBlock^>();
}
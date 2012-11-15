// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "CppUnitTest.h"
#include "UnitTestingAssertSpecializations.h"
#include "..\Hilo\MonthBlock.h"
#include "StubExceptionPolicy.h"
#include "StubYearGroup.h"
#include "StubResourceLoader.h"
#include "StubPhoto.h"
#include "StubRepository.h"
#include "..\Hilo\CalendarExtensions.h"

using namespace concurrency;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Hilo;
using namespace Platform;
using namespace Windows::Globalization;
using namespace Platform::Collections;

namespace HiloTests
{
    TEST_CLASS(MonthBlockTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            auto us = ref new Vector<String^>();
            us->Append("us-EN");
            auto cal = ref new Calendar(us, CalendarIdentifiers::Gregorian, ClockIdentifiers::TwelveHour);
            cal->Day = 23;
            cal->Month = 5;
            cal->Year = 2012;
            cal->Hour = 1;
            cal->Minute = 30;
            cal->Second = 0;
            cal->Nanosecond = 0;
            m_expectedDate = cal->GetDateTime();  
            m_yearGroup = ref new StubYearGroup(m_expectedDate);
            m_resourceLoader = ref new StubResourceLoader();
            m_exceptionPolicy = std::make_shared<StubExceptionPolicy>();
            m_repository = std::make_shared<StubRepository>(m_exceptionPolicy);
        }
 
        TEST_METHOD(MonthBlockShouldGetTitleForMonthFromLocalizedSource)
        {
            auto model = ref new MonthBlock(m_yearGroup, 12, nullptr, m_repository, m_exceptionPolicy);

            auto name = model->Name;

            int year, month;

            CalendarExtensions::WriteLocalizedYearAndMonth(m_expectedDate, year, month);
            auto expectedName = CalendarExtensions::GetLocalizedAbbreviatedMonthName(year, 12);

            Assert::AreEqual(expectedName, name);
        }

        TEST_METHOD(MonthBlockShouldSetMonthFromConstructor)
        {
            unsigned int expected = 12U;
            auto model = ref new MonthBlock(m_yearGroup, expected, nullptr, m_repository, m_exceptionPolicy);

            auto month = model->Month;

            Assert::AreEqual(expected, month);
        }

    private:
        Windows::Foundation::DateTime m_expectedDate;
        StubYearGroup^ m_yearGroup;
        StubResourceLoader^ m_resourceLoader;
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
        std::shared_ptr<StubRepository> m_repository;
    };
}
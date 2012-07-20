//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "CppUnitTest.h"
#include "..\Hilo\ImageNavigationData.h"
#include "StubPhoto.h"
#include "StubPhotoGroup.h"

using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Windows::Globalization;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::Foundation;

namespace HiloTests
{
    TEST_CLASS(ImageNavigationDataTests)
    {
    public:
        TEST_METHOD(ImageNavigationDataShouldInitilizeFileNameAndDateFromPhoto)
        {
            StubPhoto^ photo = ref new StubPhoto();
            photo->Path = "Foo";
            photo->DateTaken = GetDateTaken();
            String^ expectedPath = "Foo";
            DateTime expectedDate = GetDateTaken();

            ImageNavigationData data(photo);

            Assert::AreEqual(expectedDate.UniversalTime, data.GetFileDate().UniversalTime);
            Assert::AreEqual(expectedPath, data.GetFilePath());
        }

        TEST_METHOD(ImageNavigationDataShouldReturnDateQueryForFileSystem)
        {
            StubPhoto^ photo = ref new StubPhoto();
            photo->Path = "Foo";
            photo->DateTaken = GetDateTaken();
            String^ expectedPath = "Foo";
            DateTime expectedDate = GetDateTaken();
            Calendar cal;
            cal.SetDateTime(expectedDate);
            int lastDay = cal.LastDayInThisMonth;
            int firstDay = cal.FirstDayInThisMonth;
            cal.Day = firstDay;
            DateTimeFormatter dtf(YearFormat::Full,
                MonthFormat::Numeric, 
                DayFormat::Default, 
                DayOfWeekFormat::None);
            String^ firstDate = dtf.Format(cal.GetDateTime());        
            cal.Day = lastDay;
            String^ lastDate = dtf.Format(cal.GetDateTime());
            std::wstringstream dateRange;
            dateRange << L"System.ItemDate:" << firstDate->Data() << ".." << lastDate->Data();
            auto expectedDateQuery = ref new String(dateRange.str().c_str());
            ImageNavigationData data(photo);

            auto query = data.GetDateQuery();

            Assert::AreEqual(expectedDateQuery, query);
        }

    private:
        static DateTime GetDateTaken()
        {
            Calendar cal;
            cal.Day = 23;
            cal.Month = 5;
            cal.Year = 2012;
            cal.Hour = 1;
            cal.Minute = 30;
            cal.Second = 0;
            cal.Nanosecond = 0;
            return cal.GetDateTime();
        }
    };
}
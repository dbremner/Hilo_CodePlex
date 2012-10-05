#include "pch.h"
#include "CppUnitTest.h"
#include "UnitTestingAssertSpecializations.h"
#include "..\Hilo\ImageNavigationData.h"
#include "StubPhoto.h"
#include "StubPhotoGroup.h"
#include "..\Hilo\CalendarExtensions.h"

using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Globalization;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::Foundation;
using namespace Windows::System::UserProfile;

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
            String^ expectedDateQuery = CalendarExtensions::CreateMonthRangeFromDate(expectedDate);

            ImageNavigationData data(photo);

            auto query = data.GetDateQuery();

            Assert::AreEqual(expectedDateQuery, query);
        }

    private:
        static DateTime GetDateTaken()
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
            return cal->GetDateTime();         
        }
    };
}
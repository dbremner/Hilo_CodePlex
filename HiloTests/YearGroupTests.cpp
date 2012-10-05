#include "pch.h"
#include "CppUnitTest.h"
#include "..\Hilo\YearGroup.h"
#include "..\Hilo\IMonthBlock.h"
#include "StubExceptionPolicy.h"
#include "StubRepository.h"

using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Windows::Globalization;

namespace HiloTests
{
    TEST_CLASS(YearGroupTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_calendar = ref new Calendar();
        }

        // Note: Assumes en-US language and locale
        TEST_METHOD(YearGroupShouldPassSelfToMonth)
        {
            m_calendar->Year = 1999;
            m_calendar->Month = 1;
            m_calendar->Day = 1;
            auto yearDate = m_calendar->GetDateTime();

            auto group = ref new YearGroup(yearDate, nullptr, nullptr, nullptr);
            auto monthBlock = group->Items->GetAt(0);
            Assert::AreEqual(group->Title, monthBlock->Group->Title);
        }

    private:
        Windows::Globalization::Calendar ^m_calendar;
    };
}
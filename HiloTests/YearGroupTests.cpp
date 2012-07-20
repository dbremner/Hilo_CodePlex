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
#include "..\Hilo\YearGroup.h"
#include "..\Hilo\IMonthBlock.h"
#include "StubExceptionPolicy.h"
#include "StubMonthBlockQuery.h"

using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;

namespace HiloTests
{
    TEST_CLASS(YearGroupTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_query = std::make_shared<StubMonthBlockQuery>();
            m_exceptionPolicy = std::make_shared<StubExceptionPolicy>();
        }

        TEST_METHOD(YearGroupShouldSetTitleForNamePassedIntoConstructor)
        {
            String^ expected = "1999";

            auto group = ref new YearGroup(expected, m_query, m_exceptionPolicy);

            Assert::AreEqual(expected, group->Title);
        }

        TEST_METHOD(YearGroupShouldSetYearForNamePassedIntoConstructor)
        {
            // we have to this because the folder 
            String^ name = " 1999";
            unsigned int expected = 1999U;

            auto group = ref new YearGroup(name, m_query, m_exceptionPolicy);

            Assert::AreEqual(expected, group->Year);
        }

        TEST_METHOD(YearGroupShouldCreateTwelveMonthBlocksForYear)
        {
            String^ name = " 1999";
            auto group = ref new YearGroup(name, m_query, m_exceptionPolicy);

            unsigned int months = group->Items->Size;

            Assert::AreEqual(months, 12U);
        }

        TEST_METHOD(YearGroupShouldPassSelfToMonth)
        {
            String^ name = " 1999";
            IYearGroup^ group = ref new YearGroup(name, m_query, m_exceptionPolicy);

            auto monthBlock = group->Items->GetAt(0);

            Assert::AreEqual(group->Title, monthBlock->Group->Title);
        }

    private:
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
        std::shared_ptr<StubMonthBlockQuery> m_query;
    };
}
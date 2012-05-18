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
#include "..\Hilo\DateTimeToStringConverter.h"

using namespace Hilo::Converters;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Globalization;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::System::UserProfile;

namespace HiloTests
{
    TEST_CLASS(DateTimeToStringConverterTests)
    {
    public:

        TEST_METHOD(DateTimeConverterCanConvertDateTimeToString)
        {
            auto dtf = ref new DateTimeFormatter("shortdate", GlobalizationPreferences::Languages);

            Calendar^ calendar = ref new Calendar();
            calendar->Now();
            DateTime dateTime = calendar->ToDateTime();
            String^ stringDateTime = dtf->Format(dateTime);

            auto dateTimeConverter = ref new DateTimeToStringConverter();
            TypeName dateTimeTypeName = { "DateTime", TypeKind::Metadata } ;
            String^ value = safe_cast<String^>(dateTimeConverter->Convert(dateTime, dateTimeTypeName, nullptr, "en-US"));

            Assert::AreEqual(value, stringDateTime);
        }

        TEST_METHOD(DateTimeConverterReturnsUnsetValueIfDateTimeIsNull)
        {
            auto dateTimeConverter = ref new DateTimeToStringConverter();
            TypeName dateTimeTypeName = { "DateTime", TypeKind::Metadata } ;
            auto value = dateTimeConverter->Convert(nullptr, dateTimeTypeName, nullptr, "en-US");

            Assert::AreEqual(value, DependencyProperty::UnsetValue);
        }
    };
}

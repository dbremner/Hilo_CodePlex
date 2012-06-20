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
#include "..\Hilo\FileSizeConverter.h"

using namespace Hilo;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Windows::UI::Xaml::Interop;

namespace HiloTests
{
	TEST_CLASS(FileSizehConverterTests)
	{
	public:		
		TEST_METHOD(FileSizeConverterCanConvertSizeToBytes)
		{
            auto fileSizeConverter = ref new FileSizeConverter();
            TypeName fileSizeTypeName = { "String", TypeKind::Metadata };
            unsigned long long valueToConvert = 123;
            auto value = safe_cast<String^>(fileSizeConverter->Convert(valueToConvert, fileSizeTypeName, nullptr, "en-US"));
            Assert::AreEqual(value, "123 B");
		}

        TEST_METHOD(FileSizeConverterCanConvertSizeToKiloBytes)
		{
            auto fileSizeConverter = ref new FileSizeConverter();
            TypeName fileSizeTypeName = { "String", TypeKind::Metadata };
            unsigned long long valueToConvert = 1234;
            auto value = safe_cast<String^>(fileSizeConverter->Convert(valueToConvert, fileSizeTypeName, nullptr, "en-US"));
            Assert::AreEqual(value, "1.21 KB");
		}

        TEST_METHOD(FileSizeConverterCanConvertSizeToMegaBytes)
		{
            auto fileSizeConverter = ref new FileSizeConverter();
            TypeName fileSizeTypeName = { "String", TypeKind::Metadata };
            unsigned long long valueToConvert = 1234567;
            auto value = safe_cast<String^>(fileSizeConverter->Convert(valueToConvert, fileSizeTypeName, nullptr, "en-US"));
            Assert::AreEqual(value, "1.18 MB");
		}
	};
}

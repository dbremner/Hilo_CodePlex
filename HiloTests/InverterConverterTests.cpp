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
#include "..\Hilo\InverterConverter.h"

using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Windows::UI::Xaml::Interop;

namespace HiloTests
{
    TEST_CLASS(InverterConverterTests)
    {
    public:	
        TEST_METHOD(InverterConverterCanConvertInvertTrue)
        {
            auto inverterConverter = ref new InverterConverter();
            TypeName inverterTypeName = { "bool", TypeKind::Primitive };
            auto value = safe_cast<bool>(inverterConverter->Convert(true, inverterTypeName, nullptr, "en-US"));
            Assert::AreEqual(value, false);
        }

        TEST_METHOD(InverterConverterCanConvertInvertFalse)
        {
            auto inverterConverter = ref new InverterConverter();
            TypeName inverterTypeName = { "bool", TypeKind::Primitive };
            auto value = safe_cast<bool>(inverterConverter->Convert(false, inverterTypeName, nullptr, "en-US"));
            Assert::AreEqual(value, true);
        }
    };
}

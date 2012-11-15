// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "CppUnitTest.h"
#include "..\Hilo\BooleanToBrushConverter.h"

using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Windows::UI::Xaml::Interop;

namespace HiloTests
{
    TEST_CLASS(BooleanToBrushConverterTests)
    {
    public:
        TEST_METHOD(BooleanToBrushConverterCanConvertTrueToBrush)
        {
            auto booleanBrushConverter = ref new BooleanToBrushConverter();
            TypeName booleanBrushTypeName = { "Brush", TypeKind::Metadata };
            auto value = safe_cast<String^>(booleanBrushConverter->Convert(true, booleanBrushTypeName, nullptr, "en-US"));          
            Assert::AreEqual(value, "#FFF19720");
        }

        TEST_METHOD(BooleanToBrushConverterCanConvertFalseToBrush)
        {
            auto booleanBrushConverter = ref new BooleanToBrushConverter();
            TypeName booleanBrushTypeName = { "Brush", TypeKind::Metadata };
            auto value = safe_cast<String^>(booleanBrushConverter->Convert(false, booleanBrushTypeName, nullptr, "en-US"));           
            Assert::AreEqual(value, "#E2E2E2");
        }
    };
}

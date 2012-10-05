#include "pch.h"
#include "CppUnitTest.h"
#include "..\Hilo\FileSizeConverter.h"
#include "StubResourceLoader.h"

using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Windows::UI::Xaml::Interop;

namespace HiloTests
{
    TEST_CLASS(FileSizehConverterTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_resourceLoader = ref new StubResourceLoader();
        }

        TEST_METHOD(FileSizeConverterCanConvertSizeToBytes)
        {
            m_resourceLoader->StringToReturn = "B";
            auto fileSizeConverter = ref new FileSizeConverter(m_resourceLoader);
            TypeName fileSizeTypeName = { "String", TypeKind::Metadata };
            uint64 valueToConvert = 123;
            auto value = safe_cast<String^>(fileSizeConverter->Convert(valueToConvert, fileSizeTypeName, nullptr, "en-US"));
            Assert::AreEqual(value, "123 B");
        }

        TEST_METHOD(FileSizeConverterCanConvertSizeToKiloBytes)
        {
            m_resourceLoader->StringToReturn = "KB";
            auto fileSizeConverter = ref new FileSizeConverter(m_resourceLoader);
            TypeName fileSizeTypeName = { "String", TypeKind::Metadata };
            uint64 valueToConvert = 1234;
            auto value = safe_cast<String^>(fileSizeConverter->Convert(valueToConvert, fileSizeTypeName, nullptr, "en-US"));
            Assert::AreEqual(value, "1.21 KB");
        }

        TEST_METHOD(FileSizeConverterCanConvertSizeToMegaBytes)
        {
            m_resourceLoader->StringToReturn = "MB";
            auto fileSizeConverter = ref new FileSizeConverter(m_resourceLoader);
            TypeName fileSizeTypeName = { "String", TypeKind::Metadata };
            uint64 valueToConvert = 1234567;
            auto value = safe_cast<String^>(fileSizeConverter->Convert(valueToConvert, fileSizeTypeName, nullptr, "en-US"));
            Assert::AreEqual(value, "1.18 MB");
        }

    private:
        StubResourceLoader^ m_resourceLoader;
    };
}

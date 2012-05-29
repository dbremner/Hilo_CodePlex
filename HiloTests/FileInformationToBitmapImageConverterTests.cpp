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
#include "..\Hilo\FileInformationToBitmapImageConverter.h"

using namespace Hilo;

using namespace Platform;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace concurrency;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::Storage::BulkAccess;

namespace HiloTests
{
	TEST_CLASS(FileInformationToBitmapImageConverterTests)
	{
	public:	
		TEST_METHOD(FileInformationConverterCanConvertFileInformationToBitmapImage)
		{
            TestImageGenerator imageGenerator;
            Object^ bitmap;

            TestHelper::RunUISynced([this, &bitmap, &imageGenerator]()
            {
                auto finalFile = std::make_shared<FileInformation^>(nullptr);
                task_status status;

                auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                    .then([finalFile](FileInformation^ file)
                {
                    (*finalFile) = file;
                });   

                TestHelper::RunSynced(t2, status);
                auto fileInformationConverter = ref new FileInformationToBitmapImageConverter();
                TypeName bitmapImageTypeName = { "BitmapImage", TypeKind::Metadata };
                bitmap = fileInformationConverter->Convert(*finalFile, bitmapImageTypeName, nullptr, "en-US");
            });
            
            Assert::IsNotNull(bitmap);

            TestHelper::RunUISynced([&imageGenerator]()
            {                
                task_status status;
                TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
            });
		}

        TEST_METHOD(FileInformationConverterReturnsNullIfFileInformationIsNull)
        {
            auto fileInformationConverter = ref new FileInformationToBitmapImageConverter();
            TypeName bitmapImageTypeName = { "BitmapImage", TypeKind::Metadata };
            auto value = fileInformationConverter->Convert(nullptr, bitmapImageTypeName, nullptr, "en-US");

            Assert::IsNull(value);
        }
	};
}

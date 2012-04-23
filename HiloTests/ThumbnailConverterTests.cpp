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
#include "..\Hilo\\ThumbnailConverter.h"

using namespace Hilo::Converters;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace concurrency;
using namespace Platform;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::FileProperties;

namespace HiloTests
{
	TEST_CLASS(ThumbnailConverterTests)
	{
	public:	
		TEST_METHOD(ThumbnailConverterCanConvertThumbnailToBitmapImage)
		{
            TestImageGenerator imageGenerator;
            Object^ bitmap = nullptr;

            TestHelper::RunUISynced([this, &bitmap, &imageGenerator]()
            {
                auto finalFile = std::make_shared<FileInformation^>(nullptr);

                concurrency::task_status status;
                auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                .then([finalFile](FileInformation^ file)
                {
                    (*finalFile) = file;
                    // Ensure generation of thumbnail for test.
                    return file->GetThumbnailAsync(Windows::Storage::FileProperties::ThumbnailMode::PicturesView);
                });

                auto stream = TestHelper::RunSynced(t2, status);
                auto thumbConverter = ref new ThumbnailConverter();
                TypeName bitmapImageTypeName = { "BitmapImage", TypeKind::Metadata };
                bitmap = thumbConverter->Convert(stream, bitmapImageTypeName, nullptr, "en-US");
            });

            Assert::IsNotNull(bitmap);

            TestHelper::RunUISynced([&imageGenerator]() 
            {
                concurrency::task_status status;
                TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
            });
		}

        TEST_METHOD(ThumbnailConverterReturnsUnsetValueIfThumbnailIsNull)
        {
            auto thumbConverter = ref new ThumbnailConverter();
            TypeName bitmapImageTypeName = { "BitmapImage", TypeKind::Metadata };
            auto value = thumbConverter->Convert(nullptr, bitmapImageTypeName, nullptr, "en-US");

            Assert::AreEqual(value, DependencyProperty::UnsetValue);
        }
	};
}


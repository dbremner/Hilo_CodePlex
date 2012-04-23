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
#include <ppltasks.h>
#include "..\Hilo\HubPhoto.h"

using namespace Hilo;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Search;
using namespace Windows::Storage;

namespace HiloTests
{
    TEST_CLASS(HubPhotoTests)
    {
    public:
        TEST_METHOD(HubPhotoTestsCanRetrievePhotoPath)
        {
            TestImageGenerator imageGenerator;

            concurrency::task_status status;
            auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                .then([](FileInformation^ file) 
            {
                return ref new HubPhoto(file);
            });

            auto f = TestHelper::RunSynced<HubPhoto^>(t2, status);

            Assert::AreEqual(concurrency::completed, status);
            Assert::AreEqual("TestFile.png", f->Name);

            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
        }

        TEST_METHOD(HubPhotoTestsCanBeAssignedToAFileInformation)
        {
            TestImageGenerator imageGenerator;

            concurrency::task_status status;
            auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                .then([](FileInformation^ file) 
            {
                return ref new HubPhoto(file);
            });

            auto f = TestHelper::RunSynced<HubPhoto^>(t2, status);
            FileInformation^ fileInfo = f;
            
            Assert::AreEqual(concurrency::completed, status);
            //Assert::AreEqual("TestFile.png", f->Name);
            Assert::IsNotNull(fileInfo);
            Assert::AreEqual(f->Name, fileInfo->Name);

            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
        }

        TEST_METHOD(HubPhotoTestsCanRetrieveThumbnailAsBitmapImage)
        {
            TestImageGenerator imageGenerator;

            BitmapImage^ bitmap = nullptr;

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
                }).then([finalFile]( StorageItemThumbnail^ thumbnail)
                {
                    return (*finalFile);
                }).then([](FileInformation^ file) 
                {
                    return ref new HubPhoto(file);
                });

                auto f = TestHelper::RunSynced<HubPhoto^>(t2, status);

                bitmap = f->Thumbnail;
            });

            Assert::IsNotNull(bitmap);

            TestHelper::RunUISynced([&imageGenerator]() 
            {
                concurrency::task_status status;
                TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
            });
        }
    };
}
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
#include "..\Hilo\Photo.h"
#include "StubPhotoGroup.h"
#include "StubExceptionPolicy.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Hilo;
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
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_exceptionPolicy = ref new StubExceptionPolicy();
            m_photoGroup = ref new StubPhotoGroup("");
        }

        TEST_METHOD(PhotoTestsCanRetrievePhotoPath)
        {
            TestImageGenerator imageGenerator;

            concurrency::task_status status;
            auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                .then([this](FileInformation^ file) 
            {
                return ref new Photo(file, m_photoGroup, m_exceptionPolicy);
            });

            auto f = TestHelper::RunSynced<Photo^>(t2, status);

            Assert::AreEqual(concurrency::completed, status);
            Assert::AreEqual("TestFile.png", f->Name);

            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
        }

        //todo: f->Thumbnail is now set only on OnPropertyChanged

        //TEST_METHOD(PhotoTestsCanRetrieveThumbnailAsBitmapImage)
        //{
        //    TestImageGenerator imageGenerator;

        //    BitmapImage^ bitmap = nullptr;

        //    TestHelper::RunUISynced([this, &bitmap, &imageGenerator]()
        //    {
        //        auto finalFile = std::make_shared<FileInformation^>(nullptr);

        //        concurrency::task_status status;
        //        auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
        //            .then([finalFile](FileInformation^ file)
        //        {
        //            (*finalFile) = file;
        //            // Ensure generation of thumbnail for test.
        //            return file->GetThumbnailAsync(Windows::Storage::FileProperties::ThumbnailMode::PicturesView);
        //        }).then([finalFile]( StorageItemThumbnail^ thumbnail)
        //        {
        //            return (*finalFile);
        //        }).then([this](FileInformation^ file) 
        //        {
        //            return ref new Photo(file, m_photoGroup, m_exceptionPolicy);
        //        });

        //        auto f = TestHelper::RunSynced<Photo^>(t2, status);

        //        bitmap = f->Thumbnail;
        //    });

        //    Assert::IsNotNull(bitmap);

        //    TestHelper::RunUISynced([&imageGenerator]() 
        //    {
        //        concurrency::task_status status;
        //        TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
        //    });
        //}

    private:
        StubExceptionPolicy^ m_exceptionPolicy;
        StubPhotoGroup^ m_photoGroup;
    };
}
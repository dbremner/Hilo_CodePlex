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
#include "..\Hilo\ThumbnailGenerator.h"
#include "StubExceptionPolicy.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Windows::Storage;
using namespace Hilo;

namespace HiloTests
{		
    TEST_CLASS(ThumbnailGeneratorTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_imageGenerator = TestImageGenerator();
        }

        TEST_METHOD_CLEANUP(Cleanup)
        {
            concurrency::task_status status;
            TestHelper::RunSynced(m_imageGenerator.DeleteFilesAsync(), status);
        }

        TEST_METHOD(ThumbnailGeneratorTestsGivenValidImagesThenGeneratesCorrectNumberOfThumbnailFiles)
        {
            auto sourceImages = GenerateSourceImages();
            auto thumbnailFolder = CreateThumbnailFolder();

            ThumbnailGenerator generator(ref new StubExceptionPolicy());

            concurrency::task_status status;
            auto generatedImages = TestHelper::RunSynced(generator.Generate(sourceImages, thumbnailFolder), status);
            Assert::AreEqual(concurrency::task_status::completed, status);

            Assert::AreEqual(3U, generatedImages->Size);
        }

        TEST_METHOD(ThumbnailGeneratorTestsGivenValidImagesThenGeneratesIntoCorrectFolder)
        {
            auto sourceImages = GenerateSourceImages();
            auto thumbnailFolder = CreateThumbnailFolder();

            ThumbnailGenerator generator(ref new StubExceptionPolicy());

            concurrency::task_status status;
            auto generatedImages = TestHelper::RunSynced(generator.Generate(sourceImages, thumbnailFolder), status);
            Assert::AreEqual(concurrency::task_status::completed, status);

            auto imagePath = generatedImages->GetAt(0)->Path;
            auto folderPath = thumbnailFolder->Path;

            std::wstring imagePathString(imagePath->Data());
            std::wstring folderPathString(folderPath->Data());

            auto substring = imagePathString.substr(0, folderPathString.size());
            Assert::AreEqual(substring, folderPathString);
        }

        TEST_METHOD(ThumbnailGeneratorTestsGivenAnInvalidDoesNotReturnFile)
        {
            auto sourceImages = GenerateInvalidSourceImage();
            auto thumbnailFolder = CreateThumbnailFolder();

            ThumbnailGenerator generator(ref new StubExceptionPolicy());

            concurrency::task_status status;
            auto generatedImages = TestHelper::RunSynced(generator.Generate(sourceImages, thumbnailFolder), status);
            Assert::AreEqual(concurrency::task_status::completed, status);

            Assert::AreEqual(0U, generatedImages->Size);
        }

    private:

        Platform::Collections::Vector<StorageFile^>^ GenerateInvalidSourceImage()
        {
            concurrency::task_status status;
            auto image = TestHelper::RunSynced(
                m_imageGenerator.CreateTestImageFileFromLocalFolder("InvalidImageFile.jpg", "ThumbnailGenerator_InvalidImage.jpg"),
                status);
            Assert::AreEqual(concurrency::task_status::completed, status);

            auto sourceImages = ref new Platform::Collections::Vector<StorageFile^>();
            auto storageFileItem = dynamic_cast<StorageFile^>(image);
            sourceImages->Append(storageFileItem);

            return sourceImages;
        }

        Platform::Collections::Vector<StorageFile^>^ GenerateSourceImages()
        {
            concurrency::task_status status;
            auto images = TestHelper::RunSynced(
                m_imageGenerator.CreateTestImagesFromLocalFolder("UnitTestLogo.png", 3, "random_test"), 
                status);
            Assert::AreEqual(concurrency::task_status::completed, status);

            auto sourceImages = ref new Platform::Collections::Vector<StorageFile^>(begin(images), end(images));
            return sourceImages;
        }

        Windows::Storage::StorageFolder^ CreateThumbnailFolder()
        {
            concurrency::task_status status;
            // Windows::Storage::StorageFolder^ installStorageFolder = Windows::ApplicationModel::Package::Current->InstalledLocation;
            auto installStorageFolder = Windows::Storage::KnownFolders::PicturesLibrary;
                        
            auto createFolderTask = concurrency::create_task(installStorageFolder->CreateFolderAsync(
                "ThumbnailTestGeneratorTestFolder", 
                Windows::Storage::CreationCollisionOption::ReplaceExisting));

            auto folder = TestHelper::RunSynced(
               createFolderTask, 
               status);

            Assert::AreEqual(concurrency::task_status::completed, status);

            return folder;
        }


        TestImageGenerator m_imageGenerator;

    };
}
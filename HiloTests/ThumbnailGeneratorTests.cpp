#include "pch.h"
#include "CppUnitTest.h"
#include "UnitTestingAssertSpecializations.h"
#include "..\Hilo\ThumbnailGenerator.h"
#include "StubExceptionPolicy.h"

using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Windows::Storage;

namespace HiloTests
{
    TEST_CLASS(ThumbnailGeneratorTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_imageGenerator = TestImageGenerator();
            m_thumbnailFolder = CreateThumbnailFolder();
        }

        TEST_METHOD_CLEANUP(Cleanup)
        {
            concurrency::task_status status;
            TestHelper::RunSynced(m_imageGenerator.DeleteFilesAsync(), status);
            DeleteThumbnailFolder(m_thumbnailFolder);
        }

        TEST_METHOD(ThumbnailGeneratorTestsGivenValidImagesThenGeneratesCorrectNumberOfThumbnailFiles)
        {
            concurrency::task_status status;

            auto sourceImages = GenerateSourceImages();
            ThumbnailGenerator generator(std::make_shared<StubExceptionPolicy>());
            auto generatedImages = TestHelper::RunSynced(generator.Generate(sourceImages, m_thumbnailFolder), status);

            Assert::AreEqual(concurrency::task_status::completed, status);
            Assert::AreEqual(3U, generatedImages->Size);
        }

        TEST_METHOD(ThumbnailGeneratorTestsGivenValidImagesThenGeneratesIntoCorrectFolder)
        {
            concurrency::task_status status;

            auto sourceImages = GenerateSourceImages();
            ThumbnailGenerator generator(std::make_shared<StubExceptionPolicy>());
            auto generatedImages = TestHelper::RunSynced(generator.Generate(sourceImages, m_thumbnailFolder), status);
            Assert::AreEqual(concurrency::task_status::completed, status);

            auto imagePath = generatedImages->GetAt(0)->Path;
            auto folderPath = m_thumbnailFolder->Path;
            std::wstring imagePathString(imagePath->Data());
            std::wstring folderPathString(folderPath->Data());
            auto substring = imagePathString.substr(0, folderPathString.size());

            Assert::AreEqual(substring, folderPathString);
        }

        TEST_METHOD(ThumbnailGeneratorTestsGivenAnInvalidDoesNotReturnFile)
        {
            concurrency::task_status status;

            auto sourceImages = GenerateInvalidSourceImage();
            ThumbnailGenerator generator(std::make_shared<StubExceptionPolicy>());
            auto generatedImages = TestHelper::RunSynced(generator.Generate(sourceImages, m_thumbnailFolder), status);

            Assert::AreEqual(concurrency::task_status::completed, status);
            Assert::AreEqual(0U, generatedImages->Size);
        }

    private:
        TestImageGenerator m_imageGenerator;
        StorageFolder^ m_thumbnailFolder;

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

            auto folder = TestHelper::RunSynced(createFolderTask, status);

            Assert::AreEqual(concurrency::task_status::completed, status);

            return folder;
        }

        void DeleteThumbnailFolder(IStorageFolder^ folder)
        {
            concurrency::task_status status;

            auto deleteFolderTask = concurrency::create_task(folder->DeleteAsync());
            TestHelper::RunSynced(deleteFolderTask, status);
            Assert::AreEqual(concurrency::task_status::completed, status);
        }
    };
}
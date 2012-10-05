#include "pch.h"
#include "CppUnitTest.h"
#include "UnitTestingAssertSpecializations.h"
#include "..\Hilo\Photo.h"
#include "StubPhotoGroup.h"
#include "StubExceptionPolicy.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Hilo;
using namespace Platform;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Storage::BulkAccess;

namespace HiloTests
{
    TEST_CLASS(HubPhotoTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_exceptionPolicy = std::make_shared<StubExceptionPolicy>();
            m_photoGroup = ref new StubPhotoGroup("");
        }

        TEST_METHOD(PhotoTestsCanRetrieveName)
        {
            TestImageGenerator imageGenerator;
            concurrency::task_status status;
            auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                .then([this](FileInformation^ file) 
            {
                return ref new Photo(file, m_photoGroup, m_exceptionPolicy);
            });
            auto photo = TestHelper::RunSynced<Photo^>(t2, status);

            auto name = photo->Name;

            Assert::AreEqual(concurrency::completed, status);
            Assert::AreEqual("TestFile.png", name);
            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
        }

        TEST_METHOD(PhotoTestsCanRetrievePath)
        {
            TestImageGenerator imageGenerator;
            concurrency::task_status status;
            String^ expectedPath;
            auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                .then([this, &expectedPath](FileInformation^ file) 
            {
                expectedPath = file->Path;
                return ref new Photo(file, m_photoGroup, m_exceptionPolicy);
            });
            auto photo = TestHelper::RunSynced<Photo^>(t2, status);

            auto path = photo->Path;

            Assert::AreEqual(concurrency::completed, status);
            Assert::AreEqual(expectedPath, path);
            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
        }

        TEST_METHOD(PhotoTestsCanRetrieveFileType)
        {
            TestImageGenerator imageGenerator;
            concurrency::task_status status;
            String^ expectedFileType = ".png";
            auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                .then([this](FileInformation^ file) 
            {
                return ref new Photo(file, m_photoGroup, m_exceptionPolicy);
            });
            auto photo = TestHelper::RunSynced<Photo^>(t2, status);

            auto fileType = photo->FileType;

            Assert::AreEqual(concurrency::completed, status);
            Assert::AreEqual(expectedFileType, fileType);
            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
        }

        TEST_METHOD(PhotoTestsCanRetrieveThumbnail)
        {
            TestImageGenerator imageGenerator;
            concurrency::task_status status;
            auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                .then([this](FileInformation^ file) 
            {
                return ref new Photo(file, m_photoGroup, m_exceptionPolicy);
            });
            auto photo = TestHelper::RunSynced<Photo^>(t2, status);

            BitmapImage^ image;
            TestHelper::RunUISynced([photo, &image] 
            {
                image = photo->Thumbnail;
            });

            Assert::AreEqual(concurrency::completed, status);
            Assert::IsNotNull(image);
            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
        }

    private:
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
        StubPhotoGroup^ m_photoGroup;
    };
}
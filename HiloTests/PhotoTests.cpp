// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include <string>
#include "CppUnitTest.h"
#include "UnitTestingAssertSpecializations.h"
#include "..\Hilo\Photo.h"
#include "..\Hilo\PhotoImage.h"
#include "StubPhotoGroup.h"
#include "StubExceptionPolicy.h"

using namespace concurrency;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Hilo;
using namespace Platform;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Storage;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::Foundation;

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

        // Tests for Photo class (thumbnail images in month groups)

        TEST_METHOD(PhotoTestsCanRetrievePath)
        {
            TestImageGenerator imageGenerator;
            concurrency::task_status status;
            String^ expectedPath;
            auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                .then([this, &expectedPath](StorageFile^ file) 
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

        TEST_METHOD(PhotoTestsCanRetrieveThumbnail)
        {
            TestImageGenerator imageGenerator;
            concurrency::task_status status;
            Photo^ photo;
            BitmapImage^ secondThumbnail;
            BitmapImage^ firstThumbnail;
            EventRegistrationToken registrationToken;
            bool sawPropertyChanged = false;

            // Handler for PropertyChanged event of Photo class
            std::function<void (Object^, PropertyChangedEventArgs^)> callback = [&sawPropertyChanged](Object^ sender, PropertyChangedEventArgs^ e)
            {
                std::wstring propName(e->PropertyName->Data());
                if (propName == L"Thumbnail")
                {
                    sawPropertyChanged = true;
                }
            };

            // Returns true if the test can stop listening for PropertyChanged events
            std::function<bool ()> stopWhenFn = [&sawPropertyChanged]() { return sawPropertyChanged; };

            // Do scenario: Create a Photo and query the Thumbnail property. The result will be
            // null, but a series of async tasks will be started that eventually result in a PropertyChanged("Thumbnail") event.
            TestHelper::RunUISynced([this,  &imageGenerator, &photo, &registrationToken, &firstThumbnail, callback, &status]() 
            {
                // RecordMainThread();
                assert(IsMainThread());
                auto t1 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png").then([this, &photo, &registrationToken, &firstThumbnail, callback](StorageFile^ file) 
                    {
                        assert(IsMainThread());
                        photo = ref new Photo(file, m_photoGroup, m_exceptionPolicy);
                        registrationToken = photo->PropertyChanged::add(ref new PropertyChangedEventHandler(callback));
                        return photo->Thumbnail;  // will be null
                    }, task_continuation_context::use_current());

                firstThumbnail = TestHelper::RunSynced<BitmapImage^>(t1, status);
            });

            // Run the Dispatcher until PropertyChanged("Thumbnail") is observed.
            TestHelper::RunMessagePumpUntil(stopWhenFn);
            Assert::IsTrue(sawPropertyChanged);

            // Then, get the Thumbnail property a second time. The result will be nonnull.
            TestHelper::RunUISynced([this, &photo, &sawPropertyChanged, &secondThumbnail]()
            {
               // PropertyChanged("Thumbnail") must have occurred before the second property get operation.
               secondThumbnail = photo->Thumbnail;
            });

            Assert::IsTrue(status == concurrency::completed);
            Assert::IsNull(firstThumbnail);
            Assert::IsNotNull(secondThumbnail);

            // Clean up test resources
            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
            photo->PropertyChanged::remove(registrationToken);
        }

        // PhotoImage class

        TEST_METHOD(PhotoImageTestsCanRetrieveName)
        {
            TestImageGenerator imageGenerator;
            concurrency::task_status status;
            auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                .then([this](StorageFile^ file) 
            {
                return ref new PhotoImage(ref new Photo(file, m_photoGroup, m_exceptionPolicy), m_exceptionPolicy);
            });
            auto photo = TestHelper::RunSynced<PhotoImage^>(t2, status);
            auto name = photo->Name;

            Assert::AreEqual(concurrency::completed, status);
            Assert::AreEqual("TestFile.png", name);  
            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
        }

        TEST_METHOD(PhotoImageTestsCanRetrievePath)
        {
            TestImageGenerator imageGenerator;
            concurrency::task_status status;
            String^ expectedPath;
            auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                .then([this, &expectedPath](StorageFile^ file) 
            {
                expectedPath = file->Path;
                return ref new PhotoImage(ref new Photo(file, m_photoGroup, m_exceptionPolicy), m_exceptionPolicy);
            });
            auto photo = TestHelper::RunSynced<PhotoImage^>(t2, status);

            auto path = photo->Path;

            Assert::AreEqual(concurrency::completed, status);
            Assert::AreEqual(expectedPath, path);
            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
        }

        TEST_METHOD(PhotoImageTestsCanRetrieveFileType)
        {
            TestImageGenerator imageGenerator;
            concurrency::task_status status;
            String^ expectedFileType = ".png";
            auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                .then([this](StorageFile^ file) 
            {
                return ref new PhotoImage(ref new Photo(file, m_photoGroup, m_exceptionPolicy), m_exceptionPolicy);
            });
            auto photo = TestHelper::RunSynced<PhotoImage^>(t2, status);

            auto fileType = photo->FileType;

            Assert::AreEqual(concurrency::completed, status);
            Assert::AreEqual(expectedFileType, fileType);
            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
        }

        TEST_METHOD(PhotoImageTestsCanRetrieveThumbnail)
        {
            TestImageGenerator imageGenerator;
            concurrency::task_status status;
            PhotoImage^ photoImage;
            BitmapImage^ secondThumbnail;
            BitmapImage^ firstThumbnail;
            EventRegistrationToken registrationToken;
            bool sawPropertyChanged = false;

            // Handler for PropertyChanged event of PhotoImage class
            std::function<void (Object^, PropertyChangedEventArgs^)> callback = [&sawPropertyChanged](Object^ sender, PropertyChangedEventArgs^ e)
            {
                std::wstring propName(e->PropertyName->Data());
                if (propName == L"Thumbnail")
                {
                    sawPropertyChanged = true;
                }
            };

            // Returns true if the test can stop listening for PropertyChanged events
            std::function<bool ()> stopWhenFn = [&sawPropertyChanged]() { return sawPropertyChanged; };

            // Do scenario: Create a PhotoImage and query the Thumbnail property. The result will be
            // null, but a series of async tasks will be started that eventually result in a PropertyChanged("Thumbnail") event.
            TestHelper::RunUISynced([this,  &imageGenerator, &photoImage, &registrationToken, &firstThumbnail, callback, &status]() 
            {
                // RecordMainThread();
                assert(IsMainThread());
                auto t1 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png").then([this, &photoImage, &registrationToken, &firstThumbnail, callback](StorageFile^ file) 
                    {
                        assert(IsMainThread());
                        ref new PhotoImage(ref new Photo(file, m_photoGroup, m_exceptionPolicy), m_exceptionPolicy);
                        photoImage = ref new PhotoImage(ref new Photo(file, m_photoGroup, m_exceptionPolicy), m_exceptionPolicy);
                        registrationToken = photoImage->PropertyChanged::add(ref new PropertyChangedEventHandler(callback));
                        return photoImage->Thumbnail;  // will be null
                    }, task_continuation_context::use_current());

                firstThumbnail = TestHelper::RunSynced<BitmapImage^>(t1, status);
            });

            // Run the Dispatcher until PropertyChanged("Thumbnail") is observed.
            TestHelper::RunMessagePumpUntil(stopWhenFn);
            Assert::IsTrue(sawPropertyChanged);

            // Then, get the Thumbnail property a second time. The result will be nonnull.
            TestHelper::RunUISynced([this, &photoImage, &sawPropertyChanged, &secondThumbnail]()
            {
               // PropertyChanged("Thumbnail") must have occurred before the second property get operation.
               secondThumbnail = photoImage->Thumbnail;
            });

            Assert::IsTrue(status == concurrency::completed);
            Assert::IsNull(firstThumbnail);
            Assert::IsNotNull(secondThumbnail);

            // Clean up test resources
            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
            photoImage->PropertyChanged::remove(registrationToken);
        }

        // Verifies that the PhotoImage::InitializeAsync method initializes async properties correctly and raises the
        // required PropertyChanged events.
        TEST_METHOD(PhotoImageTestsCanRetrieveAsyncProperties)
        {
            TestImageGenerator imageGenerator;
            concurrency::task_status status;
            PhotoImage^ photoImage;

            String^ resolution;
            String^ formattedDateTaken;
            String^ formattedTimeTaken;
            uint64 fileSize;
            BitmapImage^ image;

            EventRegistrationToken registrationToken;
            int sawPropertyChanged[6] = {0, 0, 0, 0, 0, 0};
            bool sawUnknownPropertyChangedEvent = false;

            // Handler for PropertyChanged event of PhotoImage class
            std::function<void (Object^, PropertyChangedEventArgs^)> callback = [&sawPropertyChanged, &sawUnknownPropertyChangedEvent](Object^ sender, PropertyChangedEventArgs^ e)
            {
                std::wstring propName(e->PropertyName->Data());
                if (propName == L"Resolution")
                {
                    sawPropertyChanged[0]++;
                }
                else if (propName == L"FormattedDateTaken")
                {
                    sawPropertyChanged[2]++;
                }
                else if (propName == L"FormattedTimeTaken")
                {
                    sawPropertyChanged[3]++;
                }
                else if (propName == L"FileSize")
                {
                    sawPropertyChanged[4]++;
                }
                else if (propName == L"Image")
                {
                    sawPropertyChanged[5]++;
                }
                else
                {
                    sawUnknownPropertyChangedEvent = true;
                }
            };                

            // Do scenario: Create a PhotoImage, invoke its InitializeAsync method, and then retrieve properties.
            TestHelper::RunUISynced([this, &imageGenerator, &photoImage, &registrationToken, &sawPropertyChanged, &status, callback, 
                                     &resolution, &formattedDateTaken, &formattedTimeTaken, &fileSize, &image]() 
            {
                // RecordMainThread();
                assert(IsMainThread());
                auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png").then([this, &photoImage, &registrationToken, &sawPropertyChanged, callback](StorageFile^ file) 
                {
                   photoImage = ref new PhotoImage(ref new Photo(file, m_photoGroup, m_exceptionPolicy), m_exceptionPolicy);
                   registrationToken = photoImage->PropertyChanged::add(ref new PropertyChangedEventHandler(callback));
                 }, task_continuation_context::use_current()).then([&photoImage]() -> task<void>
                {
                   return create_task(photoImage->InitializeAsync());
                }, task_continuation_context::use_current()).then([&photoImage, &resolution, &formattedDateTaken, &formattedTimeTaken, &fileSize, &image]() 
                {
                    // Properties are available after InitializeAsync finishes running.
                    resolution = photoImage->Resolution;
                    formattedDateTaken = photoImage->FormattedDateTaken;
                    formattedTimeTaken = photoImage->FormattedTimeTaken;
                    fileSize = photoImage->FileSize;
                    image = photoImage->Image;

                   return true;
                });

                TestHelper::RunSynced<bool>(t2, status);
            }); 

            // After initialization completes, check that the properties have been initialized and that
            // the required PropertyChanged events were raised.
            Assert::AreEqual(1, sawPropertyChanged[0], L"PropertyChanged('Resolution') event did not fire the expected number of times.");
            Assert::AreEqual(1, sawPropertyChanged[2], L"PropertyChanged('FormattedDateTaken') event did not fire the expected number of times.");
            Assert::AreEqual(1, sawPropertyChanged[3], L"PropertyChanged('FormattedTimeTaken') event did not fire the expected number of times.");
            Assert::AreEqual(1, sawPropertyChanged[4], L"PropertyChanged('FileSize') event did not fire the expected number of times.");
            Assert::AreEqual(1, sawPropertyChanged[5], L"PropertyChanged('Image') event did not fire the expected number of times.");
            Assert::IsFalse(sawUnknownPropertyChangedEvent, L"Saw unexpected property changed event.");

            // Check that the expected initializations occurred
            Assert::AreEqual(L"150 x 150", resolution);
            Assert::IsNotNull(formattedDateTaken);
            Assert::IsNotNull(formattedTimeTaken);
            Assert::IsTrue(5789ul == fileSize);
            Assert::IsNotNull(image);

            // Clean up test
            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
            photoImage->PropertyChanged::remove(registrationToken);
        }

    private:
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
        StubPhotoGroup^ m_photoGroup;
    };
}
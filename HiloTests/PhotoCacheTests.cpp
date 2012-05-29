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
#include "..\Hilo\PhotoCache.h"
#include "..\Hilo\MonthGroup.h"
#include "..\Hilo\Photo.h"
#include "StubExceptionPolicy.h"

using namespace concurrency;
using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Globalization;

namespace HiloTests
{
    TEST_CLASS(PhotoCacheTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_exceptionPolicy = ref new StubExceptionPolicy();
        }

        TEST_METHOD(PhotoCacheInsertPhotoShouldAddPhotoForYearMonth)
        {
            concurrency::task_status status;
            TestImageGenerator imageGenerator;
            auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                .then([](FileInformation^ file) 
            {
                return file;
            });
            PhotoCache^ photoCache = ref new PhotoCache();
            auto f = TestHelper::RunSynced<FileInformation^>(t2, status);
            Assert::AreEqual(completed, status);
            auto photoGroup = ref new MonthGroup(KnownFolders::PicturesLibrary, photoCache, m_exceptionPolicy);
            auto photo = ref new Photo(f, photoGroup, m_exceptionPolicy);
            auto cal = GetCalendarForFilePhoto(f);

            photoCache->InsertPhoto(photo);

            auto actual = photoCache->GetForYearAndMonth(cal->Year, cal->Month);
            auto name = actual->Name;
            Assert::AreEqual(photo->Name, actual->Name);
            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
        }

    private:
        StubExceptionPolicy^ m_exceptionPolicy;

        ICalendar^ GetCalendarForFilePhoto(FileInformation^ f)
        {
            ICalendar^ cal = ref new Calendar();
            auto imageDate = f->ImageProperties->DateTaken;
            if (imageDate.UniversalTime == 0) 
            {
                imageDate = f->BasicProperties->DateModified;
            }
            cal->FromDateTime(imageDate);

            return cal;
        }
    };
}
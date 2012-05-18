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
#include "..\Hilo\ImageViewData.h"
#include "..\Hilo\Photo.h"
#include "StubPhotoGroup.h"

using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Windows::Globalization;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::Foundation;
using namespace Windows::Storage::BulkAccess;

namespace HiloTests
{
    TEST_CLASS(ImageViewDataTests)
    {
    public:
        TEST_METHOD(ImageViewDataShouldInitilizeFileNameAndDateFromPhoto)
        {
            TestImageGenerator imageGenerator;
            concurrency::task_status status;
            auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                .then([](FileInformation^ file) 
            {
                return ref new Photo(file, ref new StubPhotoGroup("Test"));
            });
            auto f = TestHelper::RunSynced<Photo^>(t2, status);
            FileInformation^ fileInfo = f;
            auto expectedDate = DateForFileInformation(fileInfo);
            auto expectedFilePath = fileInfo->Path;

            ImageViewData^ data = ref new ImageViewData(f);

            Assert::AreEqual(expectedDate.UniversalTime, data->FileDate.UniversalTime);
            Assert::AreEqual(expectedFilePath, data->FilePath);
            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
        }

        TEST_METHOD(ImageViewDataShouldReturnDateQueryForFileSystem)
        {
            TestImageGenerator imageGenerator;
            concurrency::task_status status;
            auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                .then([](FileInformation^ file) 
            {
                return ref new Photo(file, ref new StubPhotoGroup("Test"));
            });
            auto f = TestHelper::RunSynced<Photo^>(t2, status);
            FileInformation^ fileInfo = f;
            auto expectedDate = DateForFileInformation(fileInfo);
            Calendar cal;
            cal.FromDateTime(expectedDate);
            int lastDay = cal.LastDayInThisMonth;
            int firstDay = cal.FirstDayInThisMonth;
            cal.Day = firstDay;
            DateTimeFormatter dtf(YearFormat::Full,
                    MonthFormat::Numeric, 
                    DayFormat::Default, 
                    DayOfWeekFormat::None);
            String^ firstDate = dtf.Format(cal.ToDateTime());        
            cal.Day = lastDay;
            String^ lastDate = dtf.Format(cal.ToDateTime());
            std::wstringstream dateRange;
            dateRange << L"System.ItemDate:" << firstDate->Data() << ".." << lastDate->Data();
            auto expectedDateQuery = ref new String(dateRange.str().c_str());
            ImageViewData^ data = ref new ImageViewData(f);

            auto query = data->DateQuery;

            Assert::AreEqual(expectedDateQuery, query);
        }
    };
}
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
#include "..\Hilo\IRepository.h"
#include "..\Hilo\IQueryOperation.h"
#include "..\Hilo\IExceptionPolicy.h"
#include "StubExceptionPolicy.h"
#include "StubRepository.h"
#include "StubQueryOperation.h"
#include "StubPhoto.h"

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
            m_repository = ref new StubRepository();
            m_exceptionPolicy = ref new StubExceptionPolicy();
            m_queryOperation = ref new StubQueryOperation(nullptr);
        }

        TEST_METHOD(PhotoCacheInsertPhotoShouldAddPhotoForYearMonth)
        {
         
            PhotoCache^ photoCache = ref new PhotoCache();
            auto photo = ref new StubPhoto();
            ICalendar^ cal = ref new Calendar();
            cal->Year = 2012;
            cal->Month = 5;
            cal->Day = 5;
            photo->DateTaken = cal->GetDateTime();

            photoCache->InsertPhoto(photo);

            auto actual = photoCache->GetForYearAndMonth(cal->Year, cal->Month);
            auto name = actual->Name;
            Assert::AreEqual(photo->Name, actual->Name);
        }

    private:
        IExceptionPolicy^ m_exceptionPolicy;
        IRepository^ m_repository;
        IQueryOperation^ m_queryOperation;
    };
}
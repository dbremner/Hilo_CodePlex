﻿//===============================================================================
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
#include "StubExceptionPolicy.h"
#include "StubPhoto.h"

using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Windows::Globalization;

namespace HiloTests
{
    TEST_CLASS(PhotoCacheTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_exceptionPolicy = std::make_shared<StubExceptionPolicy>();
        }

        TEST_METHOD(PhotoCacheInsertPhotoShouldAddPhotoForYearMonth)
        {        
            PhotoCache photoCache;
            auto photo = ref new StubPhoto();
            ICalendar^ cal = ref new Calendar();
            cal->Year = 2012;
            cal->Month = 5;
            cal->Day = 5;
            photo->DateTaken = cal->GetDateTime();

            photoCache.InsertPhoto(photo);

            auto actual = photoCache.GetForYearAndMonth(cal->Year, cal->Month);
            auto name = actual->Name;
            Assert::AreEqual(photo->Name, actual->Name);
        }

    private:
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
    };
}
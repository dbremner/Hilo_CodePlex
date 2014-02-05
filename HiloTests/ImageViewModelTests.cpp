// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "CppUnitTest.h"
#include "UnitTestingAssertSpecializations.h"
#include "..\Hilo\ImageViewModel.h"
#include "StubExceptionPolicy.h"
#include "StubPhotoGroup.h"
#include "StubPhoto.h"
#include "StubRepository.h"

using namespace concurrency;
using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Globalization;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::UI::Xaml::Data;

namespace HiloTests
{
    TEST_CLASS(ImageViewModelTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_photoGroup = ref new StubPhotoGroup("");
            m_exceptionPolicy = std::make_shared<StubExceptionPolicy>();
            m_repository = std::make_shared<StubRepository>(m_exceptionPolicy);
        }

        TEST_METHOD(ImageViewModelShouldFireOnPropertyChangedWhenSettingAppBarIsSticky)
        {
            auto vm = std::make_shared<ImageViewModel^>(nullptr);
            bool propertyChangedFired = false;
            TestHelper::RunUISynced([this, vm, &propertyChangedFired]() 
            {
                (*vm) = ref new ImageViewModel(m_repository, m_exceptionPolicy);
                (*vm)->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender,  PropertyChangedEventArgs^ e) 
                {
                    propertyChangedFired = true;
                });
                (*vm)->IsAppBarSticky = true;
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::IsTrue((*vm)->IsAppBarSticky);
        }

        TEST_METHOD(ImageViewModelShouldSetupRotateCommandWhenConstructed)
        {
            auto vm = std::make_shared<ImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new ImageViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNotNull((*vm)->RotateImageCommand);       
        }

        TEST_METHOD(ImageViewModelShouldSetupCropCommandWhenConstructed)
        {
            auto vm = std::make_shared<ImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new ImageViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNotNull((*vm)->CropImageCommand); 
        }

        TEST_METHOD(ImageViewModelShouldSetupCartoonizeCommandWhenConstructed)
        {
            auto vm = std::make_shared<ImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]()
            {
                (*vm) = ref new ImageViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNotNull((*vm)->CartoonizeImageCommand);
        }

        TEST_METHOD(ImageViewModelCanGetMonthAndYear)
        {
            auto vm = std::make_shared<ImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new ImageViewModel(m_repository, m_exceptionPolicy);
            });
            auto monthAndYear = (*vm)->MonthAndYear;

            DateTime dt = (*vm)->GetStateFileDate();
            Calendar cal;
            cal.SetDateTime(dt);

            DateTimeFormatter dtf("month year");
            String^ result = dtf.Format(cal.GetDateTime());
            Assert::AreEqual(monthAndYear, result);
        }

        TEST_METHOD(ImageViewModelShouldCallRepositoryToGetPhotos)
        {
            auto vm = std::make_shared<ImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new ImageViewModel(m_repository, m_exceptionPolicy);
            });
            task_status status;

            TestHelper::RunUISynced([vm, &status]() 
            {
                TestHelper::RunSynced((*vm)->QueryPhotosAsync(), status);
            });

            Assert::AreEqual(completed, status);
            Assert::IsTrue(m_repository->GetPhotosForDateRangeQueryAsyncHasBeenCalled());
        }

        TEST_METHOD(ImageViewModelShouldCallRepositoryToGetPhoto)
        {
            auto vm = std::make_shared<ImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new ImageViewModel(m_repository, m_exceptionPolicy);
            });
            task_status status;

            TestHelper::RunSynced((*vm)->QueryPhotosAsync(), status);

            Assert::AreEqual(completed, status);
            Assert::IsTrue(m_repository->GetSinglePhotoAsyncHasBeenCalled());
        }

        TEST_METHOD(ImageViewModelRecoversFromState)
        {
            auto vm = std::make_shared<ImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new ImageViewModel(m_repository, m_exceptionPolicy);
            });
            auto state = ref new Map<String^, Object^>();
            std::wstringstream dateStream;
            Calendar cal;
            cal.SetToNow();
            auto dt = cal.GetDateTime();
            dateStream << dt.UniversalTime;
            state->Insert("query","");
            state->Insert("filePath", "path");
            state->Insert("fileDate", ref new String(dateStream.str().c_str()));

            (*vm)->LoadState(state);

            Assert::AreEqual("path", (*vm)->GetStateFilePath());
            Assert::AreEqual(dt.UniversalTime, (*vm)->GetStateFileDate().UniversalTime);
        }

        TEST_METHOD(ImageViewModelSavesToState)
        {
            auto vm = std::make_shared<ImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new ImageViewModel(m_repository, m_exceptionPolicy);
            });
            auto state = ref new Platform::Collections::Map<String^, Object^>();
            Calendar cal;
            cal.SetToNow();
            auto dt = cal.GetDateTime();
            auto photo = ref new StubPhoto();
            photo->Path = "path";
            photo->SetDateTaken(dt);
            (*vm)->Initialize("path", dt, "query");
              TestHelper::RunUISynced([vm, photo]()
            {
                (*vm)->SelectedItem = photo;
            });

            (*vm)->SaveState(state);

            Assert::IsTrue(state->HasKey("query"));
            Assert::IsTrue(state->HasKey("fileDate"));
            Assert::IsTrue(state->HasKey("filePath"));
            Assert::IsTrue(true);
        }

        TEST_METHOD(ImageViewModelShouldAddItselfAsObserverOfQuery)
        {
            auto vm = std::make_shared<ImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new ImageViewModel(m_repository, m_exceptionPolicy);
            });

            Assert::IsTrue(m_repository->GetAddObserverHasBeenCalled());
        }

    private:
        StubPhotoGroup^ m_photoGroup;
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
        std::shared_ptr<StubRepository> m_repository;
    };
}
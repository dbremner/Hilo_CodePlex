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
#include "..\Hilo\ImageViewModel.h"
#include "StubExceptionPolicy.h"
#include "StubPhotoGroup.h"
#include "StubAllPhotosQuery.h"
#include "StubSinglePhotoQuery.h"
#include "StubPhoto.h"

using namespace concurrency;
using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Globalization;
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
            m_allPhotosQuery = std::make_shared<StubAllPhotosQuery>();
            m_singlePhotoQuery = std::make_shared<StubSinglePhotoQuery>();
        }

        TEST_METHOD(ImageViewModelShouldFireOnPropertyChangedWhenSettingAppBarIsSticky)
        {
            auto vm = ref new ImageViewModel(m_singlePhotoQuery, m_allPhotosQuery, m_exceptionPolicy);
            bool propertyChangedFired = false;
            vm->PropertyChanged += ref new PropertyChangedEventHandler([&propertyChangedFired](Object^ sender,  PropertyChangedEventArgs^ e) {
                propertyChangedFired = true;
            });

            TestHelper::RunUISynced([vm]() {
                vm->IsAppBarSticky = true;
            });

            Assert::IsTrue(propertyChangedFired);
            Assert::IsTrue(vm->IsAppBarSticky);
        }

        TEST_METHOD(ImageViewModelShouldSetupRotateCommandWhenConstructed)
        {
            ImageViewModel model(m_singlePhotoQuery, m_allPhotosQuery, m_exceptionPolicy);
            Assert::IsNotNull(model.RotateImageCommand);       
        }

        TEST_METHOD(ImageViewModelShouldSetupCropCommandWhenConstructed)
        {
            ImageViewModel model(m_singlePhotoQuery, m_allPhotosQuery, m_exceptionPolicy);
            Assert::IsNotNull(model.CropImageCommand); 
        }

        TEST_METHOD(ImageViewModelCanGetMonthAndYear)
        {
            ImageViewModel model(m_singlePhotoQuery, m_allPhotosQuery, m_exceptionPolicy);
            auto monthAndYear = model.MonthAndYear;

            DateTime dt = model.GetStateFileDate();
            Calendar cal;
            cal.SetDateTime(dt);           
            String^ result = cal.MonthAsString() + " " + cal.YearAsString();

            Assert::AreEqual(monthAndYear, result);
        }

        TEST_METHOD(ImageViewModelShouldCallRepositoryToGetPhotos)
        {
            auto vm = ref new ImageViewModel(m_singlePhotoQuery, m_allPhotosQuery, m_exceptionPolicy);
            task_status status;

            TestHelper::RunUISynced([vm, &status]() {
                TestHelper::RunSynced(vm->QueryPhotosAsync(), status);
            });

            Assert::AreEqual(completed, status);
            Assert::IsTrue(m_allPhotosQuery->GetHasBeenCalled());
        }

        TEST_METHOD(ImageViewModelShouldCallRepositoryToGetPhoto)
        {
            auto vm = ref new ImageViewModel(m_singlePhotoQuery, m_allPhotosQuery, m_exceptionPolicy);
            task_status status;

            TestHelper::RunSynced(vm->QueryPhotosAsync(), status);

            Assert::AreEqual(completed, status);
            Assert::IsTrue(m_singlePhotoQuery->GetHasBeenCalled());
        }

        TEST_METHOD(ImageViewModelRecoversFromState)
        {
            auto vm = ref new ImageViewModel(m_singlePhotoQuery, m_allPhotosQuery, m_exceptionPolicy);
            auto state = ref new Map<String^, Object^>();
            std::wstringstream dateStream;
            Calendar cal;
            cal.SetToNow();
            auto dt = cal.GetDateTime();
            dateStream << dt.UniversalTime;
            state->Insert("query","");
            state->Insert("filePath", "path");
            state->Insert("fileDate",ref new String(dateStream.str().c_str()));

            vm->LoadState(state);

            Assert::AreEqual("path", vm->GetStateFilePath());
            Assert::AreEqual(dt.UniversalTime, vm->GetStateFileDate().UniversalTime);
        }

        TEST_METHOD(ImageViewModelSavesToState)
        {
            auto vm = ref new ImageViewModel(m_singlePhotoQuery, m_allPhotosQuery, m_exceptionPolicy);
            auto state = ref new Platform::Collections::Map<String^, Object^>();
            Calendar cal;
            cal.SetToNow();
            auto dt = cal.GetDateTime();
            auto photo = ref new StubPhoto();
            photo->Path = "path";
            photo->DateTaken = dt;
            vm->Initialize("path", dt, "query");
            TestHelper::RunUISynced([vm, photo]() {
                vm->SelectedItem = photo;
            });

            vm->SaveState(state);

            Assert::IsTrue(state->HasKey("query"));
            Assert::IsTrue(state->HasKey("fileDate"));
            Assert::IsTrue(state->HasKey("filePath"));
        }

        TEST_METHOD(ImageViewModelShouldAddItselfAsObserverOfQuery)
        {
            auto model = ref new ImageViewModel(m_singlePhotoQuery, m_allPhotosQuery, m_exceptionPolicy);

            Assert::IsTrue(m_allPhotosQuery->GetObserved());
        }

    private:
        StubPhotoGroup^ m_photoGroup;
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;
        std::shared_ptr<StubAllPhotosQuery> m_allPhotosQuery;
        std::shared_ptr<StubSinglePhotoQuery> m_singlePhotoQuery;
    };
}
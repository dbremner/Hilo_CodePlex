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
#include "..\Hilo\Photo.h"
#include "..\Hilo\ImageNavigationData.h"
#include "..\Hilo\ImageViewModel.h"
#include "DispatchedEvent.h"
#include "StubExceptionPolicy.h"
#include "StubPhotoGroup.h"
#include "StubRepository.h"

using namespace concurrency;
using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Navigation;

namespace HiloTests
{
    TEST_CLASS(ImageViewModelTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_exceptionPolicy = ref new StubExceptionPolicy();
            m_photoGroup = ref new StubPhotoGroup("");
            m_repository = ref new StubRepository();
        }

        TEST_METHOD(ImageViewModelShouldFireOnPropertyChangedWhenSettingAppBarIsSticky)
        {
            auto vm = ref new ImageViewModel(m_repository, m_exceptionPolicy);
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

        TEST_METHOD(ImageViewModelRotateImageCommandShouldNotBeNull)
        {
            auto vm = ref new ImageViewModel(m_repository, m_exceptionPolicy);

            ICommand^ rotateCommand = vm->RotateImageCommand;

            Assert::IsNotNull(rotateCommand, "Rotate image command should not be null");        
        }

        TEST_METHOD(ImageViewModelCropImageCommandShouldNotBeNull)
        {
            auto vm = ref new ImageViewModel(m_repository, m_exceptionPolicy);

            ICommand^ cropCommand = vm->CropImageCommand;

            Assert::IsNotNull(cropCommand, "Crop image command should not be null");        
        }

        TEST_METHOD(ImageViewModelShouldCallRepositoryToGetPhotos)
        {
            auto vm = ref new ImageViewModel(m_repository, m_exceptionPolicy);
            task_status status;

            TestHelper::RunUISynced([vm, &status]() {
                TestHelper::RunSynced(vm->QueryPhotosAsync(), status);
            });

            Assert::AreEqual(completed, status);
            Assert::IsTrue(m_repository->GetPhotosForGroupWithQueryOperationAsyncCalled);
        }

        /*TEST_METHOD(ImageViewModelShouldCallRepositoryToGetPhoto)
        {
        auto vm = ref new ImageViewModel(m_repository, m_exceptionPolicy);
        task_status status;

        TestHelper::RunSynced(vm->QueryPhotoAsync(), status);

        Assert::AreEqual(completed, status);
        Assert::IsTrue(m_repository->GetPhotoForGroupWithQueryOperationAsyncCalled);
        }*/

        // todo: re-add when the bewildering file load failure is fixed, otherwise these fail to intermittently.
        /*
        TEST_METHOD(ImageViewModelRecoversFromState)
        {
        auto vm = ref new ImageViewModel(ref new StubExceptionPolicy());

        TestImageGenerator imageGenerator;
        auto photo = GetTestPhoto(imageGenerator);

        auto state = ref new Platform::Collections::Map<String^, Object^>();

        std::wstringstream dateStream;
        dateStream << photo->DateTaken.UniversalTime;

        state->Insert("query","");
        state->Insert("filePath",photo->Path);
        state->Insert("fileDate",ref new String(dateStream.str().c_str()));

        vm->LoadState(state);

        // Forces load of item so we can verify it loads properly from state.
        concurrency::task_status status;
        TestHelper::RunSynced(vm->GetPhotoAsync(), status);
        Assert::AreEqual(concurrency::task_status::completed, status);

        auto item = vm->SelectedItem;
        Assert::IsNotNull(item);
        Assert::AreEqual(photo->Path, item->Path);
        }

        TEST_METHOD(ImageViewModelSavesToState)
        {
        auto vm = ref new ImageViewModel(ref new StubExceptionPolicy());
        auto state = ref new Platform::Collections::Map<String^, Object^>();

        TestImageGenerator imageGenerator;
        auto photo = GetTestPhoto(imageGenerator);

        auto imageViewData = ref new ImageNavigationData(photo);

        vm->Initialize(imageViewData);
        concurrency::task_status status;
        TestHelper::RunSynced(vm->GetPhotoAsync(), status);
        Assert::AreEqual(concurrency::task_status::completed, status);

        vm->SaveState(state);

        Assert::IsTrue(state->HasKey("query"));
        Assert::IsTrue(state->HasKey("fileDate"));
        Assert::IsTrue(state->HasKey("filePath"));
        }
        */

        /*TEST_METHOD(ImageViewModelShouldSetValuesFromNavigationParameterThatIsImageNavigationData)
        {
        auto vm = ref new ImageViewModel();
        TypeName pageType;
        auto photos = ref new Vector<Object^>();
        concurrency::task_status status;
        FileInformation^ file = TestHelper::RunAsync<FileInformation^>(TestHelper::CreateTestImageFileFromLocalFolder("UnitTestLogo.png"), status);
        auto imageData = ref new ImageNavigationData(file, photos);
        try
        {
        auto navArgs = ref new NavigationEventArgs(ref new Page(), imageData, pageType, ref new Uri("Test"), NavigationMode::New);
        }
        catch(Platform::Exception^ e)
        {
        auto i = e->Message;
        }

        vm->OnNavigatedTo(nullptr);

        Assert::AreSame(static_cast<Object^>(photos), vm->Photos);
        Assert::AreSame(file, vm->SelectedItem);
        }

        TEST_METHOD(ImageViewModelShouldNotThrowWhenPassedInvalidParameterForNavigation)
        {
        auto vm = ref new ImageViewModel();
        TypeName pageType;
        auto photos = ref new Vector<Object^>();
        concurrency::task_status status;
        FileInformation^ file = TestHelper::RunAsync<FileInformation^>(TestHelper::CreateTestImageFileFromLocalFolder("UnitTestLogo.png"), status);
        auto imageData = ref new ImageNavigationData(file, photos);
        auto navArgs = ref new NavigationEventArgs(nullptr, imageData, pageType, nullptr, NavigationMode::New);

        vm->OnNavigatedTo(navArgs);

        Assert::IsNull(vm->Photos);
        Assert::IsNull(vm->SelectedItem);
        }*/

    private:
        StubExceptionPolicy^ m_exceptionPolicy;
        StubPhotoGroup^ m_photoGroup;
        StubRepository^ m_repository;
    };
}
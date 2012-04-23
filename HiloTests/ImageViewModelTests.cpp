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
#include "..\Hilo\ImageViewModel.h"

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
        TEST_METHOD(ImageViewModelShouldFireOnPropertyChangedWhenSettingAppBarIsSticky)
        {
            auto vm = ref new ImageViewModel();
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
            auto vm = ref new ImageViewModel();

            ICommand^ editCommand = vm->RotateImageCommand;

            Assert::IsNotNull(editCommand, "Rotate image command should not be null");        
        }

        /*TEST_METHOD(ImageViewModelShouldSetValuesFromNavigationParameterThatIsImageViewData)
        {
            auto vm = ref new ImageViewModel();
            TypeName pageType;
            auto photos = ref new Vector<Object^>();
            concurrency::task_status status;
            FileInformation^ file = TestHelper::RunAsync<FileInformation^>(TestHelper::CreateTestImageFileFromLocalFolder("UnitTestLogo.png"), status);
            auto imageData = ref new ImageViewData(file, photos);
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
            auto imageData = ref new ImageViewData(file, photos);
            auto navArgs = ref new NavigationEventArgs(nullptr, imageData, pageType, nullptr, NavigationMode::New);

            vm->OnNavigatedTo(navArgs);

            Assert::IsNull(vm->Photos);
            Assert::IsNull(vm->SelectedItem);
        }*/
    };
}
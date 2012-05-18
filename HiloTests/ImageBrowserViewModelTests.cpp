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
#include "..\Hilo\ImageBrowserViewModel.h"
#include "..\Hilo\PhotoCache.h"
#include "..\Hilo\MonthGroup.h"
#include "..\Hilo\Photo.h"
#include "StubExceptionPolicy.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::UI::Xaml::Input;

namespace HiloTests
{
	TEST_CLASS(ImageBrowserViewModelTests)
	{
	public:
        TEST_METHOD(ImageBrowserViewModelGroupCommandShouldNotBeNull)
        {
            auto vm = ref new ImageBrowserViewModel(ref new StubExceptionPolicy());
            ICommand^ groupCommand = vm->GroupCommand;
            Assert::IsNotNull(groupCommand, "Group command should not be null");        
        }

        TEST_METHOD(ImageBrowserViewModelRotateImageCommandShouldNotBeNull)
        {
            auto vm = ref new ImageBrowserViewModel(ref new StubExceptionPolicy());
            ICommand^ rotateCommand = vm->RotateImageCommand;
            Assert::IsNotNull(rotateCommand, "Rotate image command should not be null");        
        }

        TEST_METHOD(ImageBrowserViewModelCropImageCommandShouldNotBeNull)
        {
            auto vm = ref new ImageBrowserViewModel(ref new StubExceptionPolicy());
            ICommand^ cropCommand = vm->CropImageCommand;
            Assert::IsNotNull(cropCommand, "Crop image command should not be null");        
        }

        TEST_METHOD(ImageBrowserViewModelCanSetSelectedItem)
        {
            concurrency::task_status status;
            TestImageGenerator imageGenerator;
            auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                .then([](FileInformation^ file)
            {
                return file;
            });

            auto cache = ref new PhotoCache();
            auto f = TestHelper::RunSynced<FileInformation^>(t2, status);
            Assert::AreEqual(completed, status);
            auto group = ref new MonthGroup(KnownFolders::PicturesLibrary, cache);
            Object^ photo = ref new Photo(f, group); 

            auto vm = ref new ImageBrowserViewModel(ref new StubExceptionPolicy());
            vm->SelectedItem = photo;
            Assert::AreEqual(photo, vm->SelectedItem);
     
            TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
        }


	};
}

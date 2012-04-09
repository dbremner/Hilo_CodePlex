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
#include <ppltasks.h>
#include "..\Hilo\Photo.h"

using namespace Hilo;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Search;
using namespace Windows::Storage;

namespace HiloTests
{
    TEST_CLASS(PhotoTests)
    {
    public:
        TEST_METHOD(PhotoTestsCanRetrievePhotoPath)
        {
            concurrency::task_status status;
            auto t2 = CreateTestFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                .then([](FileInformation^ file) 
            {
                return ref new Photo(file);
            });

            auto f = TestHelper::RunAsync<Photo^>(t2, status);

            Assert::AreEqual(concurrency::completed, status);
            Assert::AreEqual("TestFile.png", f->Name);
        }

        TEST_METHOD(PhotoTestsCanRetrieveThumnailAsBitmapImage)
        {

            BitmapImage^ bitmap = nullptr;

            TestHelper::RunUIAsync([this, &bitmap]()
            {
                concurrency::task_status status;
                auto t2 = CreateTestFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                    .then([](FileInformation^ file) 
                {
                    return ref new Photo(file);
                });

                auto f = TestHelper::RunAsync<Photo^>(t2, status);

                bitmap = f->Thumbnail;
            });

            Assert::IsNotNull(bitmap);
        }

        concurrency::task<FileInformation^> CreateTestFileFromLocalFolder(Platform::String^ fileName, Platform::String^ newName = "test.png")
        {
            Windows::Storage::StorageFolder^ installStorageFolder = Windows::ApplicationModel::Package::Current->InstalledLocation;
            return concurrency::task<StorageFile^>(installStorageFolder->GetFileAsync("Images\\" + fileName))
                .then([newName](StorageFile^ file)
            {
                return file->CopyAsync(Windows::Storage::KnownFolders::PicturesLibrary, newName, NameCollisionOption::ReplaceExisting);
            }, concurrency::task_continuation_context::use_current()).then([](StorageFile^ file) 
            {
                auto fileTypeFilter = ref new Vector<String^>();
                fileTypeFilter->Append(".png");
                auto picturesFolder = Windows::Storage::KnownFolders::PicturesLibrary;
                auto queryOptions = ref new QueryOptions(CommonFileQuery::OrderByDate, fileTypeFilter);
                queryOptions->FolderDepth = FolderDepth::Deep;
                queryOptions->IndexerOption = IndexerOption::DoNotUseIndexer;
                queryOptions->ApplicationSearchFilter = "System.FileName:=\"" + file->Name + "\"";
                auto fileQuery = picturesFolder->CreateFileQueryWithOptions(queryOptions);
                auto fileInformationFactory = ref new FileInformationFactory(fileQuery, ThumbnailMode::PicturesView, 200, ThumbnailOptions::UseCurrentScale , false);
                return fileInformationFactory->GetFilesAsync();
            }, concurrency::task_continuation_context::use_current()).then([](IVectorView<FileInformation^>^ files)
            {
                return files->GetAt(0);
            }, concurrency::task_continuation_context::use_current());
        }
    };
}
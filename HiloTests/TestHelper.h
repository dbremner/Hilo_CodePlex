//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once
#include "CppUnitTest.h"

namespace HiloTests
{
    class TestHelper
    {
    public:
        static void RunUIAsync(std::function<void ()>&& action);

        template <typename T>
        static T RunAsync(concurrency::task<T> t, concurrency::task_status& status, bool pump = true)
        {
            HANDLE hEvent = CreateEventEx(NULL, NULL, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
            if (hEvent == NULL)
            {
                throw std::bad_alloc();
            }

            T result;
            status = concurrency::not_complete;
            t.then([&hEvent, &status, &result](concurrency::task<T> previousTask){
                try
                {
                    result = previousTask.get();
                    status = concurrency::completed;
                }
                catch(...)
                {
                    status = concurrency::canceled;
                }
                SetEvent(hEvent);
            });

            if (pump)
            {
                DWORD dummy;
                CoWaitForMultipleHandles(0x8, INFINITE, 1, &hEvent, &dummy);
            }
            else
            {
                WaitForSingleObjectEx(hEvent, INFINITE,true);

            }
            CloseHandle(hEvent);


            return result;
        }

        static concurrency::task<Windows::Storage::BulkAccess::FileInformation^> CreateTestImageFileFromLocalFolder(
            Platform::String^ fileName, 
            Platform::String^ newName = "test.png")
        {
            Windows::Storage::StorageFolder^ installStorageFolder = Windows::ApplicationModel::Package::Current->InstalledLocation;
            return concurrency::task<Windows::Storage::StorageFile^>(installStorageFolder->GetFileAsync("Images\\" + fileName))
                .then([newName](Windows::Storage::StorageFile^ file) {
                    return file->CopyAsync(Windows::Storage::KnownFolders::PicturesLibrary, newName, Windows::Storage::NameCollisionOption::ReplaceExisting);
            }).then([](Windows::Storage::StorageFile^ file) {
                auto fileTypeFilter = ref new Platform::Collections::Vector<Platform::String^>();
                fileTypeFilter->Append(".png");
                auto picturesFolder = Windows::Storage::KnownFolders::PicturesLibrary;
                auto queryOptions = ref new Windows::Storage::Search::QueryOptions(Windows::Storage::Search::CommonFileQuery::OrderByDate, fileTypeFilter);
                queryOptions->FolderDepth = Windows::Storage::Search::FolderDepth::Deep;
                queryOptions->IndexerOption = Windows::Storage::Search::IndexerOption::DoNotUseIndexer;
                queryOptions->ApplicationSearchFilter = "System.FileName:=\"" + file->Name + "\"";
                auto fileQuery = picturesFolder->CreateFileQueryWithOptions(queryOptions);
                auto fileInformationFactory = ref new Windows::Storage::BulkAccess::FileInformationFactory(
                    fileQuery, 
                    Windows::Storage::FileProperties::ThumbnailMode::PicturesView, 
                    200, 
                    Windows::Storage::FileProperties::ThumbnailOptions::UseCurrentScale , 
                    false);
                return fileInformationFactory->GetFilesAsync();
            }).then([](Windows::Foundation::Collections::IVectorView<Windows::Storage::BulkAccess::FileInformation^>^ files) {
                return files->GetAt(0);
            });
        }

        static concurrency::task<Windows::Foundation::Collections::IVectorView<Windows::Storage::StorageFile^>^> CreateTestImagesFromLocalFolder(
            Platform::String^ fileName, 
            unsigned int count,
            Platform::String^ nameBase = "test",
            Platform::String^ extension = ".png"
            )
        {
            Windows::Storage::StorageFolder^ installStorageFolder = Windows::ApplicationModel::Package::Current->InstalledLocation;
            return concurrency::task<Windows::Storage::StorageFile^>(installStorageFolder->GetFileAsync("Images\\" + fileName))
                .then([nameBase, extension, count](Windows::Storage::StorageFile^ file) {

                    std::vector<concurrency::task<Windows::Storage::StorageFile^>> tasks;

                    for(unsigned int i=0; i < count; i++)
                    {
                        Platform::String^ newName = nameBase + i + extension;

                        tasks.push_back(concurrency::task<Windows::Storage::StorageFile^>(file->CopyAsync(Windows::Storage::KnownFolders::PicturesLibrary, newName, Windows::Storage::NameCollisionOption::ReplaceExisting)));
                    }
                    return concurrency::when_all(tasks.begin(), tasks.end());
            }).then([extension, count](std::vector<Windows::Storage::StorageFile^> files) {
                auto fileTypeFilter = ref new Platform::Collections::Vector<Platform::String^>();
                fileTypeFilter->Append(extension);
                auto picturesFolder = Windows::Storage::KnownFolders::PicturesLibrary;
                auto queryOptions = ref new Windows::Storage::Search::QueryOptions(Windows::Storage::Search::CommonFileQuery::OrderByDate, fileTypeFilter);
                queryOptions->FolderDepth = Windows::Storage::Search::FolderDepth::Deep;
                queryOptions->IndexerOption = Windows::Storage::Search::IndexerOption::DoNotUseIndexer;
                Platform::String^ filter = "";

                std::for_each(files.begin(), files.end(), [&filter](Windows::Storage::StorageFile^ file)
                {             
                    if (filter != "")
                    {
                        filter += " OR ";
                    }

                    filter += "System.FileName:=\"" + file->Name + "\"";
                });

                queryOptions->ApplicationSearchFilter = filter;
                auto fileQuery = picturesFolder->CreateFileQueryWithOptions(queryOptions);
                return fileQuery->GetFilesAsync(0, count);
            });
        }


        

        template<typename ExceptionType>
        static void ExceptionAssert(std::function<void ()> fnc)
        {
            bool thrown = false;
            try
            {
                fnc();
            }
            catch(ExceptionType e)
            {
                thrown = true;
            }
            Assert::IsTrue(thrown);
        }

    private:
        template <typename T>
        static concurrency::task_status TaskWait(concurrency::task<T>& t, bool pump = true);
    };
};
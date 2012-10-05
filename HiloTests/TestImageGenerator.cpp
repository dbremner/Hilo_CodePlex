#include "pch.h"
#include "TestImageGenerator.h"

using namespace HiloTests;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;

concurrency::task<FileInformation^> TestImageGenerator::CreateTestImageFileFromLocalFolder( 
    Platform::String^ fileName, 
    Platform::String^ newName)
{
    StorageFolder^ installStorageFolder = Windows::ApplicationModel::Package::Current->InstalledLocation;
    return concurrency::create_task(installStorageFolder->GetFileAsync("Images\\" + fileName))
        .then([newName](Windows::Storage::StorageFile^ file) {
            return file->CopyAsync(Windows::Storage::KnownFolders::PicturesLibrary, newName, Windows::Storage::NameCollisionOption::ReplaceExisting);
    }).then([](Windows::Storage::StorageFile^ file) {
        auto fileTypeFilter = ref new Platform::Collections::Vector<Platform::String^>();
        fileTypeFilter->Append(".png");
        fileTypeFilter->Append(".jpg");
        auto picturesFolder = Windows::Storage::KnownFolders::PicturesLibrary;
        auto queryOptions = ref new Windows::Storage::Search::QueryOptions(Windows::Storage::Search::CommonFileQuery::OrderByDate, fileTypeFilter);
        queryOptions->FolderDepth = Windows::Storage::Search::FolderDepth::Deep;
        queryOptions->IndexerOption = Windows::Storage::Search::IndexerOption::DoNotUseIndexer;
        queryOptions->ApplicationSearchFilter = "System.FileName:=\"" + file->Name + "\"";
        auto fileQuery = picturesFolder->CreateFileQueryWithOptions(queryOptions);
        auto fileInformationFactory = ref new Windows::Storage::BulkAccess::FileInformationFactory(
            fileQuery, 
            Windows::Storage::FileProperties::ThumbnailMode::PicturesView);
        return fileInformationFactory->GetFilesAsync();
    }).then([this](Windows::Foundation::Collections::IVectorView<Windows::Storage::BulkAccess::FileInformation^>^ files) {
        auto file = files->GetAt(0);
        m_createdFiles.push_back(file);
        return file;
    });
}

concurrency::task<void> HiloTests::TestImageGenerator::DeleteFilesAsync()
{
    std::vector<concurrency::task<void>> deleteTasks;

    for (auto filePath : m_createdFiles)
    {
        deleteTasks.push_back(concurrency::create_task(filePath->DeleteAsync()));
    }

    return concurrency::when_all(begin(deleteTasks), end(deleteTasks)).then([this]()
    {
        m_createdFiles.clear();
    });
}

concurrency::task<IVectorView<Windows::Storage::StorageFile^>^> HiloTests::TestImageGenerator::CreateTestImagesFromLocalFolder(Platform::String^ fileName, unsigned int count, Platform::String^ nameBase /*= "test"*/, Platform::String^ extension /*= ".png" */)
{
    Windows::Storage::StorageFolder^ installStorageFolder = Windows::ApplicationModel::Package::Current->InstalledLocation;
    return concurrency::create_task(installStorageFolder->GetFileAsync("Images\\" + fileName))
        .then([nameBase, extension, count](Windows::Storage::StorageFile^ file) {

            std::vector<concurrency::task<Windows::Storage::StorageFile^>> tasks;

            for(unsigned int i=0; i < count; i++)
            {
                Platform::String^ newName = nameBase + i + extension;

                tasks.push_back(concurrency::create_task(file->CopyAsync(Windows::Storage::KnownFolders::PicturesLibrary, newName, Windows::Storage::NameCollisionOption::ReplaceExisting)));
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

        for (auto file : files)
        {             
            if (filter != "")
            {
                filter += " OR ";
            }

            filter += "System.FileName:=\"" + file->Name + "\"";
        }

        queryOptions->ApplicationSearchFilter = filter;
        auto fileQuery = picturesFolder->CreateFileQueryWithOptions(queryOptions);
        return fileQuery->GetFilesAsync(0, count);
    }).then([this](IVectorView<StorageFile^>^ files)
    {
        for (auto item : files)
        {
            m_createdFiles.push_back(item);
        }

        return files;
    });

}

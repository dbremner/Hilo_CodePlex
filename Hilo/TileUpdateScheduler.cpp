//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "TaskExtensions.h"
#include "TileUpdateScheduler.h"
#include "ThumbnailGenerator.h"
#include "PhotoReader.h"
#include "RandomPhotoSelector.h"
#include "WideFiveImageTile.h"

using namespace concurrency;
using namespace std;
using namespace Platform;
using namespace Windows::ApplicationModel::Background;
using namespace Windows::Data::Xml::Dom;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Search;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Notifications;

using namespace Hilo;

// Specifies the name of the local app folder that holds the thumbnail images.
String^ ThumbnailsFolderName = "thumbnails";

// Specifies the number of photos in a batch. A batch is applied to a single 
// tile update.
const unsigned int BatchSize = 5;
// Specifies the number of batches. The Windows Runtime rotates through 
// multiple batches.
const unsigned int SetSize = 3;

void TileUpdateScheduler::ScheduleUpdateAsync()
{
    // Select random pictures from the Pictures library and copy them 
    // to a local app folder and then update the tile.
    InternalUpdateTileFromPicturesLibrary(ApplicationData::Current->LocalFolder);
}

task<void> TileUpdateScheduler::InternalUpdateTileFromPicturesLibrary(
    StorageFolder^ thumbnailsFolder)
{
    // Create a folder to hold the thumbnails.
    // The ReplaceExisting option specifies to replace the contents of 
    // any existing folder with a new, empty folder.
    task<StorageFolder^> createFolder(thumbnailsFolder->CreateFolderAsync(
        ThumbnailsFolderName, 
        CreationCollisionOption::ReplaceExisting));

    // The storage folder that holds the thumbnails.
    auto thumbnailStorageFolder = make_shared<StorageFolder^>(nullptr);

    return createFolder.then([thumbnailStorageFolder](StorageFolder^ createdFolder) 
    {
        (*thumbnailStorageFolder) = createdFolder;

        // until we get around the threading issue that forces us to copy the 
        // vector in the next step.

        // Retrieve the most recent photos from the library. We later 
        // select random photos from this collection.
        PhotoReader reader;
        return reader.GetPhotoStorageFilesAsync("", 2 * BatchSize * SetSize);
    }).then([](IVectorView<StorageFile^>^ files) 
    {
        // If we received fewer than the number in one batch,
        // return the empty collection. 
        if (files->Size < BatchSize)
        {
            return task_from_result(static_cast<IVector<StorageFile^>^>(
                ref new Vector<StorageFile^>()));
        }

        auto copiedFileInfos = ref new Vector<StorageFile^>(begin(files),end(files));
        return RandomPhotoSelector::SelectFilesAsync(copiedFileInfos->GetView(), SetSize * BatchSize);
    }).then([thumbnailsFolder, thumbnailStorageFolder](IVector<StorageFile^>^ selectedFiles)
    {
        // Return the empty collection if the previous step did not
        // produce enough photos.
        if (selectedFiles->Size == 0)
        {
            return task_from_result(ref new Vector<StorageFile^>());
        }

        ThumbnailGenerator thumbnailGenerator;
        return thumbnailGenerator.Generate(selectedFiles, *thumbnailStorageFolder);
    }).then([this](Vector<StorageFile^>^ files)
    {
        // Update the tile.
        UpdateTile(files);
    });
}

void TileUpdateScheduler::UpdateTile(IVector<StorageFile^>^ files)
{
    // Create a tile updater.
    TileUpdater^ tileUpdater = TileUpdateManager::CreateTileUpdaterForApplication();
    tileUpdater->Clear();
    
    unsigned int imagesCount = files->Size;
    unsigned int imageBatches = imagesCount / BatchSize;

    tileUpdater->EnableNotificationQueue(imagesCount > 0);

    for(unsigned int batch = 0; batch < imageBatches; batch++)
    {
        vector<wstring> imageList;

        // Add the selected images to the wide tile template.
        for(unsigned int image = 0; image < BatchSize; image++)
        {
            StorageFile^ file = files->GetAt(image + (batch * BatchSize));
            wstringstream imageSource;
            imageSource << L"ms-appdata:///local/" 
                        << ThumbnailsFolderName->Data() 
                        << L"/" 
                        << file->Name->Data();
            imageList.push_back(imageSource.str());
        }

        WideFiveImageTile wideTile;
        wideTile.SetImageFilePaths(imageList);

        // Create the notification and update the tile.
        auto notification = wideTile.GetTileNotification();
        tileUpdater->Update(notification);
    }
}
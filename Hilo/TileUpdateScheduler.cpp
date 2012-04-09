//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include <sstream>

#include "TaskExtensions.h"

#include "TileUpdateScheduler.h"
#include "ThumnailGenerator.h"
#include "PhotoReader.h"
#include "RandomPhotoSelector.h"
#include "WideTile05.h"

using namespace Hilo;

using namespace Concurrency;
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

//todo: duplicated right now...need to rationalize with ThumbnailGenerator.h
static Platform::String^ ThumbnailImagePrefix = "thumbImage_";
static Platform::String^ ThumbnailsFolderName = "thumbnails";

static unsigned int BatchSize   = 5;
static unsigned int SetSize     = 3;

void TileUpdateScheduler::ScheduleUpdatedTiles()
{
    InternalUpdateTileFromPictureLibrary(KnownFolders::PicturesLibrary,  ApplicationData::Current->LocalFolder);
}

task<void> TileUpdateScheduler::InternalUpdateTileFromPictureLibrary(StorageFolder^ picturesFolder, StorageFolder^ thumbnailsFolder)
{
    task<StorageFolder^> createFolder(thumbnailsFolder->CreateFolderAsync(ThumbnailsFolderName, Windows::Storage::CreationCollisionOption::ReplaceExisting));

    std::shared_ptr<StorageFolder^> thumbnailStorageFolder = std::make_shared<StorageFolder^>(nullptr);

    return createFolder.then([thumbnailStorageFolder](StorageFolder^ createdFolder) 
    {
        (*thumbnailStorageFolder) = createdFolder;
        PhotoReader reader;
        // TODO: Right now we're only getting a multiple of the batch and set size until we get around the threading issue that forces us to copy the vector in the
        //       next step.
        return reader.GetPhotoStorageFilesAsync("", 2 * BatchSize * SetSize );
    }).then([](IVectorView<StorageFile^>^ files) 
    {
        if (files->Size < BatchSize)
            return task_from_result(static_cast<IVector<StorageFile^>^>(ref new Vector<StorageFile^>()));

        auto copiedFileInfos = ref new Vector<StorageFile^>(begin(files),end(files));
        return RandomPhotoSelector::SelectFilesAsync(copiedFileInfos->GetView(), SetSize * BatchSize);
    }).then([thumbnailsFolder](IVector<StorageFile^>^ selectedFiles)
    {
        if (selectedFiles->Size == 0)
        {
            return task_from_result(ref new Vector<StorageFile^>());
        }

        return ThumnailGenerator::Generate(selectedFiles, thumbnailsFolder);
    }).then([this](Vector<StorageFile^>^ files)
    {
        if (files->Size == 0) return;

        UpdateTile(files);
        return;
    });
}

void TileUpdateScheduler::UpdateTile(IVector<StorageFile^>^ files)
{
    // Create a tile updater
    TileUpdater^ tileUpdater = TileUpdateManager::CreateTileUpdaterForApplication();
    tileUpdater->Clear();
    tileUpdater->EnableNotificationQueue(true);

    unsigned int imagesCount = files->Size;
    unsigned int imageBatches = imagesCount / BatchSize;

    for(unsigned int batch = 0; batch < imageBatches; batch++)
    {
        std::vector<std::wstring> imageList;

        // Update Wide tile template with the selected images
        for(unsigned int image = 0; image < BatchSize; image++)
        {
            StorageFile^ file = files->GetAt(image + (batch * BatchSize));
            std::wstringstream imageSource;
            imageSource << L"ms-appdata:///local/" << ThumbnailsFolderName->Data() << L"/" << file->Name->Data() ;
            imageList.push_back(imageSource.str());
        }

        WideTile05 wideTile;
        wideTile.SetImageFilePaths(imageList);

        auto notification = wideTile.GetTileNotification();
        auto s = notification->Content->GetXml();
        // Create the notification and update the tile
        tileUpdater->Update(notification);
    }
}




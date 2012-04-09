//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once
#include <ppltasks.h>
#include <collection.h>

namespace Hilo
{
    class TileUpdateScheduler
    {
    public:
        void TileUpdateScheduler::ScheduleUpdatedTiles();

    private:
        concurrency::task<void> TileUpdateScheduler::InternalUpdateTileFromPictureLibrary(Windows::Storage::StorageFolder^ picturesFolder, Windows::Storage::StorageFolder^ thumbnailsFolder);
        void TileUpdateScheduler::UpdateTile(Windows::Foundation::Collections::IVector<Windows::Storage::StorageFile^>^ files);
    };
}

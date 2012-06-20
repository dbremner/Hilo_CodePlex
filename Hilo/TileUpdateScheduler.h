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
    interface class IExceptionPolicy;

    class TileUpdateScheduler
    {
    public:
        TileUpdateScheduler();
        concurrency::task<void> TileUpdateScheduler::ScheduleUpdateAsync(IExceptionPolicy^ exceptionPolicy);

    private:
        concurrency::task<void> TileUpdateScheduler::InternalUpdateTileFromPicturesLibrary(Windows::Storage::StorageFolder^ thumbnailsFolder, IExceptionPolicy^ exceptionPolicy);
        void TileUpdateScheduler::UpdateTile(Windows::Foundation::Collections::IVector<Windows::Storage::StorageFile^>^ files);
    };
}

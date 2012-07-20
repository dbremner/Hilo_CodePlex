//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

namespace Hilo
{
    class ExceptionPolicy;

    class TileUpdateScheduler
    {
    public:
        TileUpdateScheduler();
        concurrency::task<void> TileUpdateScheduler::ScheduleUpdateAsync(std::shared_ptr<ExceptionPolicy> exceptionPolicy);

    private:
        concurrency::task<void> TileUpdateScheduler::InternalUpdateTileFromPicturesLibrary(Windows::Storage::StorageFolder^ thumbnailsFolder, std::shared_ptr<ExceptionPolicy> exceptionPolicy);
        void TileUpdateScheduler::UpdateTile(Windows::Foundation::Collections::IVector<Windows::Storage::StorageFile^>^ files);
    };
}

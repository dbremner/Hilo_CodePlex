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
        TileUpdateScheduler(IExceptionPolicy^ policy);
        concurrency::task<void> TileUpdateScheduler::ScheduleUpdateAsync();

    private:
        concurrency::task<void> TileUpdateScheduler::InternalUpdateTileFromPicturesLibrary(Windows::Storage::StorageFolder^ thumbnailsFolder);
        void TileUpdateScheduler::UpdateTile(Windows::Foundation::Collections::IVector<Windows::Storage::StorageFile^>^ files);

        IExceptionPolicy^ m_exceptionPolicy;
    };
}

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
    public delegate void DataChangedEventHandler();

    interface class IPhotoGroup;
    interface class IPhoto;
    interface class IPhotoCache;
    interface class IYearGroup;
    interface class IQueryOperation;

    public interface class IRepository
    {
        Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVectorView<IPhoto^>^>^ GetPhotosForGroupWithQueryOperationAsync(IPhotoGroup^ photoGroup, IQueryOperation^ operation);
        Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVectorView<IPhotoGroup^>^>^ GetMonthGroupedPhotosWithCacheAsync(IPhotoCache^ photoCache);
        Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVectorView<IYearGroup^>^>^ GetYearGroupedMonthsAsync();
        Windows::Foundation::IAsyncOperation<IPhoto^>^ GetPhotoForGroupWithQueryOperationAsync(IPhotoGroup^ photoGroup, IQueryOperation^ operation);

        event DataChangedEventHandler^ DataChanged;
    };
}
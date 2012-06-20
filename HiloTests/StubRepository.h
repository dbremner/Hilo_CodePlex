//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "..\Hilo\IRepository.h"

namespace HiloTests
{
    interface class Hilo::IPhoto;
    interface class Hilo::IPhotoGroup;
    interface class Hilo::IPhotoCache;
    interface class Hilo::IYearGroup;
    interface class Hilo::IQueryOperation;
    ref class Hilo::PhotoGroupData;

    ref class StubRepository sealed : public Hilo::IRepository
    {
    public:
        StubRepository();

        property Hilo::IPhoto^ PhotoToReturn;

        property bool GetPhotosForGroupWithQueryOperationAsyncCalled;

        property bool GetPhotoGroupDataForGroupWithQueryOperationAsyncCalled;

        property bool GetMonthGroupedPhotosWithCacheAsyncCalled;

        property bool GetYearGroupedMonthsAsyncCalled;

        property bool GetPhotoCountForQueryOperationAsyncCalled;

        property bool GetPhotoForGroupWithQueryOperationAsyncCalled;

        property bool DataChangedEventObserved;

        virtual Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVectorView<Hilo::IPhoto^>^>^ GetPhotosForGroupWithQueryOperationAsync(Hilo::IPhotoGroup^ photoGroup, Hilo::IQueryOperation^ operation);
        virtual Windows::Foundation::IAsyncOperation<Hilo::PhotoGroupData^>^ GetPhotoGroupDataForGroupWithQueryOperationAsync(Hilo::IPhotoGroup^ photoGroup, Hilo::IQueryOperation^ operation);
        virtual Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVectorView<Hilo::IPhotoGroup^>^>^ GetMonthGroupedPhotosWithCacheAsync(Hilo::IPhotoCache^ photoCache);
        virtual Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVectorView<Hilo::IYearGroup^>^>^ GetYearGroupedMonthsAsync();
        virtual Windows::Foundation::IAsyncOperation<unsigned int>^ GetPhotoCountForQueryOperationAsync(Hilo::IQueryOperation^ operation);
        virtual Windows::Foundation::IAsyncOperation<Hilo::IPhoto^>^ GetPhotoForGroupWithQueryOperationAsync(Hilo::IPhotoGroup^ photoGroup, Hilo::IQueryOperation^ operation);
        
        event Hilo::DataChangedEventHandler^ DataChanged
        {
            virtual Windows::Foundation::EventRegistrationToken add(Hilo::DataChangedEventHandler^ e);
            virtual void remove(Windows::Foundation::EventRegistrationToken);
        }

    private:
        bool m_observed;
        event Hilo::DataChangedEventHandler^ m_dataChangedEvent;
        Windows::Foundation::EventRegistrationToken m_watchToken;
    };
}
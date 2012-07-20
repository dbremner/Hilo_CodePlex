//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "IRepository.h"
#include "IPhoto.h"

namespace Hilo
{
    interface class IExceptionPolicy;

    ref class FileRepository sealed : public IRepository
    {
    public:
        FileRepository(IExceptionPolicy^ exceptionPolicy);
        virtual ~FileRepository();

        virtual Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVectorView<IPhoto^>^>^ GetPhotosForGroupWithQueryOperationAsync(IPhotoGroup^ photoGroup, IQueryOperation^ operation);
        virtual Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVectorView<IPhotoGroup^>^>^ GetMonthGroupedPhotosWithCacheAsync(IPhotoCache^ photoCache);
        virtual Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVectorView<IYearGroup^>^>^ GetYearGroupedMonthsAsync();
        virtual Windows::Foundation::IAsyncOperation<IPhoto^>^ GetPhotoForGroupWithQueryOperationAsync(IPhotoGroup^ photoGroup, IQueryOperation^ operation);

        event DataChangedEventHandler^ DataChanged
        {
            virtual Windows::Foundation::EventRegistrationToken add(DataChangedEventHandler^ e);
            virtual void remove(Windows::Foundation::EventRegistrationToken);
        }


    private:
        IExceptionPolicy^ m_exceptionPolicy;
        bool m_observed;
        Windows::Storage::Search::IStorageQueryResultBase^ m_queryResult;
        event DataChangedEventHandler^ m_dataChangedEvent;
        Windows::Foundation::EventRegistrationToken m_watchToken;

        void OnFileQueryContentsChanged(Windows::Storage::Search::IStorageQueryResultBase^ sender, Platform::Object^ e);
        void RemoveObserver();
    };
}
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
    template<typename T>
    class PhotoQueryBuilderResult
    {
    public:
        PhotoQueryBuilderResult()
        {
        }

        PhotoQueryBuilderResult(concurrency::task<Windows::Foundation::Collections::IVectorView<T>^> storageItemsTask, 
            Windows::Storage::Search::IStorageQueryResultBase^ queryResult) : m_storageItemsTask(storageItemsTask), m_queryResult(queryResult)
        {
        }

        concurrency::task<Windows::Foundation::Collections::IVectorView<T>^> GetStorageItemsTask()
        {
            return m_storageItemsTask;
        }

        Windows::Storage::Search::IStorageQueryResultBase^ GetQueryResult()
        {
            return m_queryResult;
        }

    private:
        concurrency::task<Windows::Foundation::Collections::IVectorView<T>^> m_storageItemsTask;
        Windows::Storage::Search::IStorageQueryResultBase^ m_queryResult;
    };
}
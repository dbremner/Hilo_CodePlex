//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "MonthGroup.h"
#include "Photo.h"
#include "PhotoReader.h"
#include "PhotoCache.h"
#include "IExceptionPolicy.h"
#include "TaskExceptionsExtensions.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Search;
using namespace Windows::UI::Core;

MonthGroup::MonthGroup(IStorageFolder^ storagefolder, PhotoCache^ photoCache, IExceptionPolicy^ exceptionPolicy) : m_storageFolder(storagefolder), m_weakPhotoCache(photoCache), m_exceptionPolicy(exceptionPolicy)
{
    assert(nullptr != dynamic_cast<IStorageFolderQueryOperations^>(m_storageFolder));
}

MonthGroup::operator IStorageFolder^()
{
    return m_storageFolder;
}

IObservableVector<Object^>^ MonthGroup::Items::get()
{
    if (nullptr == m_photos)
    {
        m_photos = ref new Vector<Object^>();
        IStorageFolderQueryOperations^ query = dynamic_cast<IStorageFolderQueryOperations^>(m_storageFolder);
        assert(query != nullptr);
        PhotoReader reader;
        task<IVectorView<FileInformation^>^> photosTask = reader.GetPhotosAsync(query, "", 9);

        photosTask.then([this](IVectorView<FileInformation^>^ files)
        {
            auto temp = ref new Vector<Object^>();
            bool first = true;
            PhotoCache^ cache = m_weakPhotoCache.Resolve<PhotoCache>();
            
            std::for_each(begin(files), end(files), [this, temp, cache, &first](FileInformation^ item) 
            {
                auto photo = ref new Photo(item, this, m_exceptionPolicy);
                temp->Append(photo);
                if (first)
                {
                    cache->InsertPhoto(photo);
                }
                first = false;
            });

            return temp;
        }, concurrency::task_continuation_context::use_current()).then([this](Vector<Object^>^ photos)
        {
            Array<Object^>^ many = ref new Array<Object^>(photos->Size);
            photos->GetMany(0, many);
            m_photos->ReplaceAll(many);
        }, concurrency::task_continuation_context::use_current()).then(ObserveException<void>(m_exceptionPolicy));;
    }
    return m_photos;
}

String^ MonthGroup::Title::get()
{
    if (nullptr != m_title)
    {
        return m_title;
    }
    return m_storageFolder->Name;
}
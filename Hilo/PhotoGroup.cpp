//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "PhotoGroup.h"
#include "Photo.h"
#include "PhotoReader.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Search;
using namespace Windows::UI::Core;

PhotoGroup::PhotoGroup(IStorageFolder^ storagefolder, bool supportSpan) : m_storageFolder(storagefolder), m_supportSpan(supportSpan) 
{
    assert(nullptr != dynamic_cast<IStorageFolderQueryOperations^>(m_storageFolder));
}

PhotoGroup::PhotoGroup(IStorageFolder^ storagefolder, bool supportSpan, String^ title) : m_storageFolder(storagefolder), m_supportSpan(supportSpan), m_title(title)
{
    assert(nullptr != dynamic_cast<IStorageFolderQueryOperations^>(m_storageFolder));
}


PhotoGroup::operator IStorageFolder^()
{
    return m_storageFolder;
}

IObservableVector<Object^>^ PhotoGroup::Items::get()
{
    if (nullptr == m_photos)
    {
        m_photos = ref new Vector<Object^>();
        IStorageFolderQueryOperations^ query = dynamic_cast<IStorageFolderQueryOperations^>(m_storageFolder);
        assert(query != nullptr);
        PhotoReader reader;
        task<IVectorView<FileInformation^>^> photosTask = reader.GetPhotosAsync(query, "");
        photosTask.then([this](IVectorView<FileInformation^>^ files)
        {
            auto temp = ref new Vector<Object^>();
            bool first = true;
            std::for_each(begin(files), end(files), [this, temp, &first](FileInformation^ item) 
            {
                auto photo = ref new Photo(item, this);
                if (first && m_supportSpan)
                {
                    photo->ColumnSpan = 2;
                    photo->RowSpan = 2;
                    first = false;
                }                
                temp->Append(photo);
            });
            return temp;
        }).then([this](Vector<Object^>^ photos)
        {
            Array<Object^>^ many = ref new Array<Object^>(photos->Size);
            photos->GetMany(0, many);
            m_photos->ReplaceAll(many);
        }, concurrency::task_continuation_context::use_current());
    }
    return m_photos;
}

String^ PhotoGroup::Title::get()
{
    if (nullptr != m_title)
    {
        return m_title;
    }
    return m_storageFolder->Name;
}
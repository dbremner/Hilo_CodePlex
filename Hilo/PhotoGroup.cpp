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

PhotoGroup::PhotoGroup(IStorageFolder^ storagefolder) : m_storageFolder(storagefolder) 
{
    assert(nullptr != dynamic_cast<IStorageFolderQueryOperations^>(m_storageFolder));
}

PhotoGroup::PhotoGroup(IStorageFolder^ storagefolder, String^ title) : m_storageFolder(storagefolder), m_title(title)
{
    assert(nullptr != dynamic_cast<IStorageFolderQueryOperations^>(m_storageFolder));
}

Object^ PhotoGroup::Items::get()
{
    if (m_photos == nullptr)
    {
        m_photos = ref new Vector<Object^>();
        IStorageFolderQueryOperations^ query = dynamic_cast<IStorageFolderQueryOperations^>(m_storageFolder);
        PhotoReader reader;
        task<IVectorView<FileInformation^>^> photosTask = reader.GetPhotosAsync(query, "");
        photosTask.then([this](IVectorView<FileInformation^>^ files)
        {
            bool first = true;
            std::for_each(begin(files), end(files), [this, &first](FileInformation^ item) 
            {
                auto photo = ref new HubPhoto(item);
                if (first)
                {
                    photo->ColumnSpan = 2;
                    photo->RowSpan = 2;
                    first = false;
                }                
                m_photos->Append(photo);
            });
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
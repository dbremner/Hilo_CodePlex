//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "HubPhotoGroup.h"
#include "Photo.h"
#include "TaskExceptionsExtensions.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::UI::Core;

HubPhotoGroup::HubPhotoGroup(
    String^ title, 
    String^ emptyTitle, 
    IAsyncOperation<IVectorView<FileInformation^>^>^ task,
    IExceptionPolicy^ exceptionPolicy) 
    : m_title(title), m_emptyTitle(emptyTitle),  m_task(task), m_retrievedPhotos(false), m_exceptionPolicy(exceptionPolicy)
{
}

HubPhotoGroup::operator IStorageFolder^()
{
    return nullptr;
}

IObservableVector<Object^>^ HubPhotoGroup::Items::get()
{
    if (m_photos == nullptr)
    {
        m_photos = ref new Vector<Object^>();
        m_task.then([this](IVectorView<FileInformation^>^ files) 
        {
            auto temp = ref new Vector<Object^>();
            bool first = true;
            std::for_each(begin(files), end(files), [this, temp, &first](FileInformation^ item) 
            {
                auto photo = ref new Photo(item, this, m_exceptionPolicy);
                if (first)
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
            m_retrievedPhotos = true;
            Array<Object^>^ many = ref new Array<Object^>(photos->Size);
            photos->GetMany(0, many);
            m_photos->ReplaceAll(many);
            OnPropertyChanged("Items");
            OnPropertyChanged("Title");
        }, concurrency::task_continuation_context::use_current())
            .then(ObserveException<void>(m_exceptionPolicy));
    }
    return m_photos;
}

String^ HubPhotoGroup::Title::get()
{
    if (!m_retrievedPhotos)
    {
        return "";
    }

    if (m_photos->Size == 0)
    {
        return m_emptyTitle;
    }
    return m_title;
}
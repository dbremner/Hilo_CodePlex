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
#include "IPhoto.h"
#include "IResizable.h"
#include "Photo.h"
#include "IRepository.h"
#include "SimpleQueryOperation.h"
#include "TaskExceptionsExtensions.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Search;
using namespace Windows::UI::Core;

const unsigned int MaxNumberOfPictures = 6;

HubPhotoGroup::HubPhotoGroup(Platform::String^ title, Platform::String^ emptyTitle, IRepository^ repository, IExceptionPolicy^ exceptionPolicy) : 
    m_title(title), m_emptyTitle(emptyTitle), m_retrievedPhotos(false), m_repository(repository), m_exceptionPolicy(exceptionPolicy)
{
    m_dataToken = m_repository->DataChanged += ref new DataChangedEventHandler(this, &HubPhotoGroup::OnDataChanged);
}

HubPhotoGroup::~HubPhotoGroup()
{
    m_repository->DataChanged -= m_dataToken;
}

IObservableVector<IPhoto^>^ HubPhotoGroup::Items::get()
{
    if (m_photos == nullptr)
    {
        OnDataChanged();
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

task<void> HubPhotoGroup::QueryPhotosAsync()
{
    // Only need to call this once and have to check since this can be called from multiple places.
    if (m_photos == nullptr)
    {
        m_photos = ref new Vector<IPhoto^>();
    }

    task<IVectorView<IPhoto^>^> t = create_task(m_repository->GetPhotosForGroupWithQueryOperationAsync(this, ref new SimpleQueryOperation(nullptr, MaxNumberOfPictures)));

    return t.then([this](IVectorView<IPhoto^>^ photos)
    {
        m_retrievedPhotos = true;
        if (photos->Size > 0)
        {
            Array<IPhoto^>^ many = ref new Array<IPhoto^>(photos->Size);
            photos->GetMany(0, many);
            m_photos->ReplaceAll(many);
            IResizable^ firstPhoto = dynamic_cast<IResizable^>(m_photos->GetAt(0));
            if (nullptr != firstPhoto)
            {
                firstPhoto->ColumnSpan = 2;
                firstPhoto->RowSpan = 2;
            }
            OnPropertyChanged("Items");
            
        }
        OnPropertyChanged("Title");
    });
}

void HubPhotoGroup::OnDataChanged()
{
    QueryPhotosAsync().then(ObserveException<void>(m_exceptionPolicy));
}
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
#include "IPhotoCache.h"
#include "IExceptionPolicy.h"
#include "IRepository.h"
#include "IQueryOperation.h"
#include "IResourceLoader.h"
#include "PhotoGroupData.h"
#include "TaskExceptionsExtensions.h"

#define MaxNumberOfThumbnailsPerMonthGroup 9

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

MonthGroup::MonthGroup(String^ title, IPhotoCache^ photoCache, IRepository^ repository, IQueryOperation^ queryOperation, IExceptionPolicy^ exceptionPolicy) : m_title(title), m_weakPhotoCache(photoCache), m_repository(repository), m_queryOperation(queryOperation), m_exceptionPolicy(exceptionPolicy)
{
    m_dataToken = m_repository->DataChanged += ref new DataChangedEventHandler(this, &MonthGroup::OnDataChanged);
}

MonthGroup::~MonthGroup()
{
    m_repository->DataChanged -= m_dataToken;
}

IObservableVector<IPhoto^>^ MonthGroup::Items::get()
{
    if (nullptr == m_photos)
    {
        OnDataChanged();
    }
    return m_photos;
}

bool MonthGroup::HasPhotos::get()
{
    return m_count > 0;
}

task<void> MonthGroup::QueryPhotosAsync()
{
    // Only need to call this once and have to check since this can be called from multiple places.
    if (nullptr == m_photos)
    {
        m_photos = ref new Vector<IPhoto^>();
    }

    auto photosTask = create_task(m_repository->GetPhotoGroupDataForGroupWithQueryOperationAsync(this, m_queryOperation));
    m_photos->Clear();
    return photosTask.then([this](PhotoGroupData^ photoGroupData)
    {
        auto temp = ref new Vector<IPhoto^>();
        bool first = true;
        IPhotoCache^ cache = m_weakPhotoCache.Resolve<IPhotoCache>();
        m_count = photoGroupData->Size;
        auto photos = photoGroupData->Photos;
        for (auto item : photos)
        {
            m_photos->Append(item);
            //temp->Append(item);
            if (first)
            {
                cache->InsertPhoto(item);
            }
            first = false;
        }

        return temp;
    }).then([this](Vector<IPhoto^>^ photos)
    {
        Array<IPhoto^>^ many = ref new Array<IPhoto^>(photos->Size);
        photos->GetMany(0, many);
        //m_photos->ReplaceAll(many);
    });
}

String^ MonthGroup::Title::get()
{
    std::wstringstream title;
    title << m_title->Data();
    if (m_count > 0)
    {
        title << L" (" << m_count.ToString()->Data() << L")";
    }
    return ref new String(title.str().c_str());
}

void MonthGroup::OnDataChanged()
{
    QueryPhotosAsync().then([this]
    {
        OnPropertyChanged("Items");
        OnPropertyChanged("Title");
        OnPropertyChanged("HasPhotos");
    }).then(ObserveException<void>(m_exceptionPolicy));
}
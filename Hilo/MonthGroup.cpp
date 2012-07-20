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
#include "TaskExceptionsExtensions.h"
#include "PhotoCache.h"
#include "MonthGroupQuery.h"
#include "IPhoto.h"
#include "ExceptionPolicy.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace std;
using namespace Windows::Foundation::Collections;

const unsigned int MaxNumberOfPictures = 8;

MonthGroup::MonthGroup(String^ title, shared_ptr<PhotoCache> photoCache, shared_ptr<MonthGroupQuery> query, shared_ptr<ExceptionPolicy> exceptionPolicy) : m_title(title), m_weakPhotoCache(photoCache), m_query(query), m_exceptionPolicy(exceptionPolicy)
{
}

IObservableVector<IPhoto^>^ MonthGroup::Items::get()
{
    if (nullptr == m_photos)
    {
        if (!m_runningQuery)
        {
            m_runningQuery = true;
            QueryPhotosAsync().then([this]
            {
                OnPropertyChanged("Items");
                OnPropertyChanged("Title");
                OnPropertyChanged("HasPhotos");
                m_runningQuery = false;
            }).then(ObserveException<void>(m_exceptionPolicy));
        }
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

    m_photos->Clear();
    auto photosTask = m_query->GetPhotoDataForMonthGroup(this, MaxNumberOfPictures, cancellation_token::none());
    return photosTask.then([this](PhotoGroupData photoGroupData)
    {
        assert(IsMainThread());
        bool first = true;
        shared_ptr<PhotoCache> cache = m_weakPhotoCache.lock();
        m_count = photoGroupData.GetSize();
        auto photos = photoGroupData.GetPhotos();
        for (auto item : photos)
        {
            m_photos->Append(item);
            if (first)
            {
                cache->InsertPhoto(item);
            }
            first = false;
        }
    });
}

String^ MonthGroup::Title::get()
{
    wstringstream title;
    title << m_title->Data();
    if (m_count > 0)
    {
        title << L" (" << m_count.ToString()->Data() << L")";
    }
    return ref new String(title.str().c_str());
}
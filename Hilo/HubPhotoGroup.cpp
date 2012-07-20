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
#include "TaskExceptionsExtensions.h"
#include "IPhoto.h"
#include "IResizable.h"
#include "PictureHubGroupQuery.h"
#include <wrl.h>

using namespace concurrency;
using namespace Hilo;
using namespace std;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Microsoft::WRL;

const unsigned int MaxNumberOfPictures = 6;

HubPhotoGroup::HubPhotoGroup(String^ title, String^ emptyTitle, shared_ptr<PictureHubGroupQuery> query, shared_ptr<ExceptionPolicy> exceptionPolicy) : 
    m_title(title), m_emptyTitle(emptyTitle), m_retrievedPhotos(false), m_query(query), m_exceptionPolicy(exceptionPolicy)
{
    IInspectable* wr(reinterpret_cast<IInspectable*>(this));
    function<void()> callback = [wr] {
        Object^ obj = reinterpret_cast<Object^>(wr);
        auto vm = dynamic_cast<HubPhotoGroup^>(obj);
        if (nullptr != vm)
        {
            vm->OnDataChanged();
        }
    };
    m_query->AddObserver(callback);
}

HubPhotoGroup::~HubPhotoGroup()
{
    if (nullptr != m_query)
    {
        m_query->RemoveObserver();
    }
}

IObservableVector<IPhoto^>^ HubPhotoGroup::Items::get()
{
    assert(IsMainThread());
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

    m_photos->Clear();
    auto t = m_query->GetPhotosForPictureHubGroupAsync(this, MaxNumberOfPictures, cancellation_token::none());
    return t.then([this](IVectorView<IPhoto^>^ photos)
    {
        assert(IsMainThread());
        m_retrievedPhotos = true;
        bool firstPhoto = true;
        for (auto photo : photos)
        {
            if (firstPhoto)
            {
                IResizable^ resizable = dynamic_cast<IResizable^>(photo);
                if (nullptr != resizable)
                {
                    resizable->ColumnSpan = 2;
                    resizable->RowSpan = 2;
                }
                firstPhoto = false;
            }
            m_photos->Append(photo);
        }
    });
}

void HubPhotoGroup::OnDataChanged()
{
    assert(IsMainThread());
    if (m_runningQuery)
    {
        m_receivedChangeWhileRunning = true;
    }

    if (!m_runningQuery)
    {
        m_runningQuery = true;
        QueryPhotosAsync().then([this]
        {
            assert(IsMainThread());
            if (!m_receivedChangeWhileRunning)
            {
                OnPropertyChanged("Items");
                OnPropertyChanged("Title");
            }
            m_runningQuery = false;
            if (m_receivedChangeWhileRunning)
            {
                m_receivedChangeWhileRunning = false;
                OnDataChanged();
            }
        }).then(ObserveException<void>(m_exceptionPolicy));
    }
}
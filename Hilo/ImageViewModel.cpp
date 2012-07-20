//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ImageNavigationData.h"
#include "ImageViewModel.h"
#include "IPhoto.h"
#include "IPhotoGroup.h"
#include "DelegateCommand.h"
#include "TaskExceptionsExtensions.h"
#include "SinglePhotoQuery.h"
#include "AllPhotosQuery.h"
#include <wrl.h>

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace std;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Globalization;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Navigation;
using namespace Microsoft::WRL;

String^ const QueryMapKey = "query";
String^ const FilePathMapKey = "filePath";
String^ const FileDateMapKey = "fileDate";

ImageViewModel::ImageViewModel(std::shared_ptr<SinglePhotoQuery> singlePhotoQuery, shared_ptr<AllPhotosQuery> allPhotosQuery, shared_ptr<ExceptionPolicy> exceptionPolicy) : ViewModelBase(exceptionPolicy), m_singlePhotoQuery(singlePhotoQuery), m_allPhotosQuery(allPhotosQuery)
{
    m_cropImageCommand = ref new DelegateCommand(
        ref new ExecuteDelegate(this, &ImageViewModel::CropImage), 
        ref new CanExecuteDelegate(this, &ImageViewModel::CanCropOrRotateImage));
    m_rotateImageCommand = ref new DelegateCommand(
        ref new ExecuteDelegate(this, &ImageViewModel::RotateImage), 
        ref new CanExecuteDelegate(this, &ImageViewModel::CanCropOrRotateImage));

    IInspectable* wr(reinterpret_cast<IInspectable*>(this));
    function<void()> callback = [wr] {
        Object^ obj = reinterpret_cast<Object^>(wr);
        auto vm = dynamic_cast<ImageViewModel^>(obj);
        if (nullptr != vm)
        {
            vm->OnDataChanged();
        }
    };
    m_allPhotosQuery->AddObserver(callback);
}

ImageViewModel::~ImageViewModel()
{
    if (nullptr != m_allPhotosQuery)
    {
        m_allPhotosQuery->RemoveObserver();
    }
}

ICommand^ ImageViewModel::CropImageCommand::get()
{
    return m_cropImageCommand;
}

ICommand^ ImageViewModel::RotateImageCommand::get()
{
    return m_rotateImageCommand;
}

IObservableVector<IPhoto^>^ ImageViewModel::Photos::get()
{
    if (m_photos == nullptr)
    {
        OnDataChanged();
    }
    return m_photos;
}

task<void> ImageViewModel::QueryPhotosAsync()
{
    assert(IsMainThread());
    if (m_photos == nullptr)
    {
        m_photos = ref new Vector<IPhoto^, PhotoPathComparer>();
    }

    m_photosCts = cancellation_token_source();
    cancellation_token token = m_photosCts.get_token();
    auto t = m_allPhotosQuery->GetPhotosForDateRangeQueryAsync(m_query, token);
    return t.then([this](IVectorView<IPhoto^>^ photos)
    {
        assert(IsBackgroundThread());
        auto temp = ref new Vector<IPhoto^>();
        for (auto photo : photos)
        {
            temp->Append(photo);
        };
        return temp;
    }, task_continuation_context::use_arbitrary()).then([this](Vector<IPhoto^>^ temp)
    {
        assert(IsMainThread());
        Array<IPhoto^>^ many = ref new Array<IPhoto^>(temp->Size);
        temp->GetMany(0, many);
        m_photos->ReplaceAll(many);
    }).then([this](task<void> priorTask)
    {
        assert(IsMainThread());
        try 
        {
            priorTask.get();
        }
        catch(const concurrency::task_canceled &)
        {
            // Didn't load the photos so reset the underlying items.
            m_photos = nullptr;
        };
    });
}

Object^ ImageViewModel::SelectedItem::get()
{
    if (ApplicationView::Value != ApplicationViewState::Snapped)
    {
        if (nullptr == m_photo && !m_runningQuerySinglePhotoAsync)
        {
            m_runningQuerySinglePhotoAsync = true;
            QuerySinglePhotoAsync().then([this](task<void> priorTask)
            {
                assert(IsMainThread());
                m_runningQuerySinglePhotoAsync = false;
                priorTask.get();
                OnPropertyChanged("SelectedItem");
            }).then(ObserveException<void>(m_exceptionPolicy));
        }
    }
    return m_photo;
}

void ImageViewModel::SelectedItem::set(Object^ value)
{
    IPhoto^ newPhoto = dynamic_cast<IPhoto^>(value);
    PhotoPathComparer compare;
    bool equal = compare(m_photo, newPhoto);

    if (ApplicationView::Value == ApplicationViewState::Snapped)
    {
        if (!equal && !m_runningQuerySinglePhotoAsync)
        {
            m_photo = newPhoto;
            m_cropImageCommand->CanExecute(nullptr);
            m_rotateImageCommand->CanExecute(nullptr);
            OnPropertyChanged("SelectedItem");
        }
    }
    else
    {
        if (!equal && nullptr != newPhoto && !m_runningQuerySinglePhotoAsync)
        {
            ClearCachedData();
            m_photo = newPhoto;
            OnPropertyChanged("SelectedItem");
        }
    }
}

String^ ImageViewModel::MonthAndYear::get()
{
    Calendar cal;
    cal.SetDateTime(m_fileDate);
    std::wstringstream monthAndYear;
    monthAndYear << cal.MonthAsString()->Data() << " " << cal.YearAsString()->Data();
    return ref new String(monthAndYear.str().c_str());
}

void ImageViewModel::SaveState(IMap<String^, Object^>^ stateMap)
{
    if (m_photo != nullptr)
    {
        std::wstringstream stringSerialization;
        stringSerialization << m_photo->DateTaken.UniversalTime ;

        stateMap->Insert(FilePathMapKey, m_photo->Path);
        stateMap->Insert(FileDateMapKey, ref new String(stringSerialization.str().c_str()));
        stateMap->Insert(QueryMapKey, m_query);
    }
}

void ImageViewModel::LoadState(IMap<String^, Object^>^ stateMap)
{
    if (stateMap != nullptr)
    {
        auto filePath = dynamic_cast<String^>(stateMap->Lookup(FilePathMapKey));

        auto fileDateString = dynamic_cast<String^>(stateMap->Lookup(FileDateMapKey));
        DateTime fileDate;
        fileDate.UniversalTime = _wtoi64(fileDateString->Data());

        auto query = dynamic_cast<String^>(stateMap->Lookup(QueryMapKey));

        Initialize(filePath, fileDate, query);
    }
}

task<void> ImageViewModel::QuerySinglePhotoAsync()
{
    assert(IsMainThread());

    m_photoCts.cancel();
    m_photoCts = cancellation_token_source();
    auto token = m_photoCts.get_token();

    if (m_filePath == nullptr) return create_task([]{});

    auto photoTask = m_singlePhotoQuery->GetPhotoAsync(m_filePath, token);
    return photoTask.then([this](IPhoto^ photo)
    {
        assert(IsMainThread());
        if (photo == nullptr)
        {
            GoHome();
            cancel_current_task();
        }
        m_photo = photo;
    }).then([this](task<void> priorTask)
    {
        assert(IsMainThread());
        try
        {
            priorTask.get();
        }
        catch (const concurrency::task_canceled&)
        {
            m_photo = nullptr;
        }
    });
}

void ImageViewModel::OnNavigatedTo(NavigationEventArgs^ e)
{
    auto data = dynamic_cast<String^>(e->Parameter);
    ImageNavigationData imageData(data);
    Initialize(imageData.GetFilePath(), imageData.GetFileDate(), imageData.GetDateQuery());
}

void ImageViewModel::OnNavigatedFrom(NavigationEventArgs^ e)
{
    m_photoCts.cancel();
    m_photosCts.cancel();
    ClearCachedData();
}

void ImageViewModel::Initialize(String^ filePath, DateTime fileDate, String^ query)
{
    m_filePath = filePath;
    m_fileDate = fileDate;
    m_query = query;
    m_photos = nullptr;
    m_photo = nullptr;
    m_runningQuerySinglePhotoAsync = false;
    m_runningQueryPhotosAsync = false;
}

String^ ImageViewModel::GetStateFilePath()
{
    return m_filePath;
}

String^ ImageViewModel::GetStateQuery()
{
    return m_query;
}

DateTime ImageViewModel::GetStateFileDate()
{
    return m_fileDate;
}

void ImageViewModel::CropImage(Object^ parameter)
{
    ImageNavigationData data(m_photo);
    ViewModelBase::GoToPage(PageType::Crop, data.SerializeToString());
}

void ImageViewModel::RotateImage(Object^ parameter)
{
    ImageNavigationData data(m_photo);
    ViewModelBase::GoToPage(PageType::Rotate, data.SerializeToString());
}

bool ImageViewModel::CanCropOrRotateImage(Object^ paratmer)
{
    return (nullptr != m_photo);
}

void ImageViewModel::OnDataChanged()
{
    assert(IsMainThread());

    if (m_runningQueryPhotosAsync)
    {
        m_receivedChangeWhileRunning = true;
    }

    if (!m_runningQueryPhotosAsync)
    {
        m_runningQueryPhotosAsync = true;
        QueryPhotosAsync().then([this]
        {
            assert(IsMainThread());
            if (!m_receivedChangeWhileRunning)
            {
                OnPropertyChanged("Photos");
                OnPropertyChanged("SelectedItem");
            }
            m_runningQueryPhotosAsync = false;
            if (m_receivedChangeWhileRunning)
            {
                m_receivedChangeWhileRunning = false;
                OnDataChanged();
            }
        }).then(ObserveException<void>(m_exceptionPolicy));
    }
}

void ImageViewModel::ClearCachedData()
{
    if (m_photo != nullptr)
    {
        m_photo->ClearImageData();
    }
}
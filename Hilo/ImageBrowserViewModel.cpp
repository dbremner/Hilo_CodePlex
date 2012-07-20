//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ImageBrowserViewModel.h"
#include "IPhoto.h"
#include "IMonthBlock.h"
#include "IYearGroup.h"
#include "IPhotoGroup.h"
#include "PhotoCache.h"
#include "DelegateCommand.h"
#include "ImageNavigationData.h"
#include "TaskExceptionsExtensions.h"
#include "VirtualMonthFoldersQuery.h"
#include "VirtualYearFoldersQuery.h"
#include <wrl.h>

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace std;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Navigation;
using namespace Microsoft::WRL;

ImageBrowserViewModel::ImageBrowserViewModel(shared_ptr<VirtualMonthFoldersQuery> monthFoldersQuery, shared_ptr<VirtualYearFoldersQuery> yearFoldersQuery, shared_ptr<ExceptionPolicy> exceptionPolicy) : m_inProgress(true), 
    m_photoCache(std::make_shared<PhotoCache>()), 
    ViewModelBase(exceptionPolicy), 
    m_monthFoldersQuery(monthFoldersQuery), 
    m_yearFoldersQuery(yearFoldersQuery),
    m_monthCancelTokenSource(cancellation_token_source()),
    m_yearCancelTokenSource(cancellation_token_source())
{
    m_groupCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &ImageBrowserViewModel::NavigateToGroup), nullptr);
    m_cropImageCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &ImageBrowserViewModel::CropImage), ref new CanExecuteDelegate(this, &ImageBrowserViewModel::CanCropOrRotateImage));
    m_rotateImageCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &ImageBrowserViewModel::RotateImage), ref new CanExecuteDelegate(this, &ImageBrowserViewModel::CanCropOrRotateImage));

    IInspectable* wr(reinterpret_cast<IInspectable*>(this));
    function<void()> callback = [wr] {
        Object^ obj = reinterpret_cast<Object^>(wr);
        auto vm = dynamic_cast<ImageBrowserViewModel^>(obj);
        if (nullptr != vm)
        {
            vm->OnDataChanged();
        }
    };
    m_monthFoldersQuery->AddObserver(callback);
}

ImageBrowserViewModel::~ImageBrowserViewModel()
{
    if (nullptr != m_monthFoldersQuery)
    {
        m_monthFoldersQuery->RemoveObserver();
    }
}

ICommand^ ImageBrowserViewModel::GroupCommand::get()
{
    return m_groupCommand;
}

ICommand^ ImageBrowserViewModel::CropImageCommand::get()
{
    return m_cropImageCommand;
}

ICommand^ ImageBrowserViewModel::RotateImageCommand::get()
{
    return m_rotateImageCommand;
}

bool ImageBrowserViewModel::InProgress::get()
{
    return m_inProgress;
}

Object^ ImageBrowserViewModel::SelectedItem::get()
{
    return m_photo;
}

void ImageBrowserViewModel::SelectedItem::set(Object^ value)
{
    if (m_photo != value)
    {
        m_photo = dynamic_cast<IPhoto^>(value);
        m_cropImageCommand->CanExecute(nullptr);
        m_rotateImageCommand->CanExecute(nullptr);
        OnPropertyChanged("SelectedItem");
    }
}

IObservableVector<IPhotoGroup^>^ ImageBrowserViewModel::MonthGroups::get()
{
    if (nullptr == m_monthGroups)
    {
        OnDataChanged();
    }
    return m_monthGroups;
}

task<void> ImageBrowserViewModel::QueryMonthGroupsAsync()
{
    assert(IsMainThread());

    if (nullptr == m_monthGroups)
    {
        m_monthGroups = ref new Vector<IPhotoGroup^>();
    }

    m_monthGroups->Clear();
    m_monthCancelTokenSource = cancellation_token_source();
    cancellation_token token = m_monthCancelTokenSource.get_token();
    auto t = m_monthFoldersQuery->GetMonthGroupedPhotosWithCacheAsync(m_photoCache, token);
    return t.then([this](IVectorView<IPhotoGroup^>^ groups)
    {
        assert(IsMainThread());

        if (groups->Size == 0)
        {
            cancel_current_task();
        }        
        for (auto group : groups)
        {
            m_monthGroups->Append(group);
        }
    });
}

IObservableVector<IYearGroup^>^ ImageBrowserViewModel::YearGroups::get()
{
    if (nullptr == m_yearGroups)
    {
        OnDataChanged();
    }
    return m_yearGroups;
}

task<void> ImageBrowserViewModel::QueryYearGroupsAsync()
{
    assert(IsMainThread());
    if (nullptr == m_yearGroups)
    {
        m_yearGroups = ref new Vector<IYearGroup^>();
    }

    m_yearGroups->Clear();
    m_yearCancelTokenSource = cancellation_token_source();
    cancellation_token token = m_yearCancelTokenSource.get_token();
    auto t = m_yearFoldersQuery->GetYearGroupedMonthsAsync(token);
    return t.then([this](IVectorView<IYearGroup^>^ groups)
    {
        assert(IsMainThread());
        if (groups->Size == 0)
        {
            cancel_current_task();
        }

        for (auto year : groups)
        {
            m_yearGroups->Append(year);
        }
    });
}

IPhoto^ ImageBrowserViewModel::GetPhotoForYearAndMonth(unsigned int year, unsigned int month)
{
    return m_photoCache->GetForYearAndMonth(year, month);
}

void ImageBrowserViewModel::OnNavigatedFrom(NavigationEventArgs^ e)
{
    assert(IsMainThread());
    m_active = false;
    m_monthCancelTokenSource.cancel();
    m_yearCancelTokenSource.cancel();
}

void ImageBrowserViewModel::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
    assert(IsMainThread());
    m_active = true;
    if (m_receivedChangeWhileNotActive)
    {
        m_receivedChangeWhileNotActive = false;
        OnDataChanged();
    }
}

void ImageBrowserViewModel::NavigateToGroup(Object^ parameter)
{
    auto group = dynamic_cast<IPhotoGroup^>(parameter);
    assert(group != nullptr);
    if (group->Items->Size > 0)
    {
        auto photo = dynamic_cast<IPhoto^>(group->Items->GetAt(0));
        assert(photo != nullptr);
        ImageNavigationData imageData(photo);
        ViewModelBase::GoToPage(PageType::Image, imageData.SerializeToString());
    }
}

bool ImageBrowserViewModel::CanCropOrRotateImage(Object^ paratmer)
{
    return (nullptr != m_photo);
}

void ImageBrowserViewModel::CropImage(Object^ parameter)
{
    ImageNavigationData imageData(m_photo);
    ViewModelBase::GoToPage(PageType::Crop, imageData.SerializeToString());
}

void ImageBrowserViewModel::RotateImage(Object^ parameter)
{
    ImageNavigationData imageData(m_photo);
    ViewModelBase::GoToPage(PageType::Rotate, imageData.SerializeToString());
}

void ImageBrowserViewModel::OnDataChanged()
{
    assert(IsMainThread());
    if (!m_active)
    {
        m_receivedChangeWhileNotActive = true;
        return;
    }

    if (m_runningMonthQuery || m_runningYearQuery)
    {
        m_receivedChangeWhileRunning = true;
        return;
    }

    RunMonthQuery();
    RunYearQuery();
}

void ImageBrowserViewModel::RunMonthQuery()
{
    if (m_runningMonthQuery)
    {
        return;
    }

    m_runningMonthQuery = true;
    ShowProgress(true);
    QueryMonthGroupsAsync().then([this](task<void> priorTask)
    {
        assert(IsMainThread());
        try
        {
            priorTask.get();
            if (!m_receivedChangeWhileRunning)
            {
                ShowProgress(false);
                OnPropertyChanged("MonthGroups");
            }
            m_runningMonthQuery = false;
            if (m_receivedChangeWhileRunning)
            {
                m_receivedChangeWhileRunning = false;
                OnDataChanged();
            }
        }
        catch (const concurrency::task_canceled&)
        {
            m_monthGroups = nullptr;
            ShowProgress(false);
            GoHome();
        }
    }, task_continuation_context::use_current()).then(ObserveException<void>(m_exceptionPolicy));
}

void ImageBrowserViewModel::RunYearQuery()
{
    if (m_runningYearQuery)
    {
        return;
    }
    m_runningYearQuery = true;
    QueryYearGroupsAsync().then([this](task<void> priorTask)
    {
        assert(IsMainThread());
        try
        {
            priorTask.get();
            if (!m_receivedChangeWhileRunning)
            {
                OnPropertyChanged("YearGroups");
            }
            m_runningYearQuery = false;
            if (m_receivedChangeWhileRunning)
            {
                m_receivedChangeWhileRunning = false;
                OnDataChanged();
            }
        }
        catch (const concurrency::task_canceled&)
        {
            m_yearGroups = nullptr;
            ShowProgress(false);
            GoHome();
        }
    }, task_continuation_context::use_current()).then(ObserveException<void>(m_exceptionPolicy));
}

void ImageBrowserViewModel::ShowProgress(bool showProgress)
{
    assert(IsMainThread());
    m_inProgress = showProgress;
    OnPropertyChanged("InProgress");
}
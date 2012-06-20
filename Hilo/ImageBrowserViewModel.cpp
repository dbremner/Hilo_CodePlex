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
#include "IRepository.h"
#include "IPhoto.h"
#include "IMonthBlock.h"
#include "IYearGroup.h"
#include "IPhotoGroup.h"
#include "PhotoCache.h"
#include "DelegateCommand.h"
#include "ImageNavigationData.h"
#include "TaskExceptionsExtensions.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Navigation;

ImageBrowserViewModel::ImageBrowserViewModel(IRepository^ repository, IExceptionPolicy^ exceptionPolicy) : m_inProgress(true), m_photoCache(ref new PhotoCache()), ViewModelBase(exceptionPolicy), m_repository(repository)
{
    m_groupCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &ImageBrowserViewModel::NavigateToGroup), nullptr);
    m_cropImageCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &ImageBrowserViewModel::CropImage), ref new CanExecuteDelegate(this, &ImageBrowserViewModel::CanCropOrRotateImage));
    m_rotateImageCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &ImageBrowserViewModel::RotateImage), ref new CanExecuteDelegate(this, &ImageBrowserViewModel::CanCropOrRotateImage));
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
        m_inProgress = true;
        OnPropertyChanged("InProgress");
        QueryMonthGroupsAsync().then([this]
        {
            m_inProgress = false;
            OnPropertyChanged("MonthGroups");
            OnPropertyChanged("InProgress");
        }).then(ObserveException<void>(m_exceptionPolicy));
    }
    return m_monthGroups;
}

task<void> ImageBrowserViewModel::QueryMonthGroupsAsync()
{
    m_monthCancelTokenSource = cancellation_token_source();
    cancellation_token token = m_monthCancelTokenSource.get_token();
    m_monthGroups = ref new Vector<IPhotoGroup^>();

    auto t = create_task(m_repository->GetMonthGroupedPhotosWithCacheAsync(m_photoCache), token);
    return t.then([this](IVectorView<IPhotoGroup^>^ groups)
    {
        Array<IPhotoGroup^>^ many = ref new Array<IPhotoGroup^>(groups->Size);
        groups->GetMany(0, many);
        m_monthGroups->ReplaceAll(many);
    }).then([this](task<void> priorTask)
    {
        try
        {
            priorTask.get();
        }
        catch (const concurrency::task_canceled&)
        {
            m_monthGroups = nullptr;
        }
    });
}

IObservableVector<IYearGroup^>^ ImageBrowserViewModel::YearGroups::get()
{
    if (nullptr == m_yearGroups)
    {
        m_inProgress = true;
        QueryYearGroupsAsync().then([this]
        {
            OnPropertyChanged("YearGroups");
        }).then(ObserveException<void>(m_exceptionPolicy));
    }
    return m_yearGroups;
}

task<void> ImageBrowserViewModel::QueryYearGroupsAsync()
{
    m_yearCancelTokenSource = cancellation_token_source();
    cancellation_token token = m_yearCancelTokenSource.get_token();
    m_yearGroups = ref new Vector<IYearGroup^>();

    auto t = create_task(m_repository->GetYearGroupedMonthsAsync(), token);
    return t.then([this](IVectorView<IYearGroup^>^ groups)
    {
        Array<IYearGroup^>^ many = ref new Array<IYearGroup^>(groups->Size);
        groups->GetMany(0, many);
        m_yearGroups->ReplaceAll(many);
    }).then([this](task<void> priorTask)
    {
        try
        {
            priorTask.get();
        }
        catch (const concurrency::task_canceled&)
        {
            m_yearGroups = nullptr;
        }
    });
}

IPhoto^ ImageBrowserViewModel::GetPhotoForYearAndMonth(unsigned int year, unsigned int month)
{
    return m_photoCache->GetForYearAndMonth(year, month);
}

void ImageBrowserViewModel::OnNavigatedFrom(NavigationEventArgs^ e)
{
    m_monthCancelTokenSource.cancel();
    m_yearCancelTokenSource.cancel();
}

void ImageBrowserViewModel::NavigateToGroup(Object^ parameter)
{
    auto group = dynamic_cast<IPhotoGroup^>(parameter);
    assert(group != nullptr);
    if (group->Items->Size > 0)
    {
        auto photo = dynamic_cast<IPhoto^>(group->Items->GetAt(0));
        assert(photo != nullptr);
        auto imageData = ref new ImageNavigationData(photo);
        ViewModelBase::GoToPage(PageType::Image, imageData->SerializeToString());
    }
}

bool ImageBrowserViewModel::CanCropOrRotateImage(Object^ paratmer)
{
    return (nullptr != m_photo);
}

void ImageBrowserViewModel::CropImage(Object^ parameter)
{
    auto imageData = ref new ImageNavigationData(m_photo);
    ViewModelBase::GoToPage(PageType::Crop, imageData->SerializeToString());
}

void ImageBrowserViewModel::RotateImage(Object^ parameter)
{
    auto imageData = ref new ImageNavigationData(m_photo);
    ViewModelBase::GoToPage(PageType::Rotate, imageData->SerializeToString());
}

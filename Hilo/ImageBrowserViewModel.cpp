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
#include "PhotoReader.h"
#include "MonthGroup.h"
#include "YearGroup.h"
#include "Photo.h"
#include "DelegateCommand.h"
#include "ImageViewData.h"
#include "PhotoCache.h"
#include "IPhotoGroup.h"
#include "TaskExceptionsExtensions.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Navigation;

ImageBrowserViewModel::ImageBrowserViewModel(IExceptionPolicy^ exceptionPolicy) : m_inProgress(true), m_photoCache(ref new PhotoCache()), ViewModelBase(exceptionPolicy)
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

bool ImageBrowserViewModel::IsAppBarEnabled::get()
{
    return m_isAppBarEnabled;
}

void ImageBrowserViewModel::IsAppBarEnabled::set(bool value)
{
    if (m_isAppBarEnabled != value)
    {
        m_isAppBarEnabled = value;
        OnPropertyChanged("IsAppBarEnabled");
    }
}

Photo^ ImageBrowserViewModel::SelectedItem::get()
{
    return m_photo;
}

void ImageBrowserViewModel::SelectedItem::set(Photo^ value)
{
    if (m_photo != value)
    {
        m_photo = value;
        m_cropImageCommand->CanExecute(nullptr);
        m_rotateImageCommand->CanExecute(nullptr);
        IsAppBarEnabled = value != nullptr;
        OnPropertyChanged("SelectedItem");
    }
}

Object^ ImageBrowserViewModel::MonthGroups::get()
{
    if (nullptr == m_monthGroups)
    {
        m_cancelTokenSource = cancellation_token_source();
        cancellation_token token = m_cancelTokenSource.get_token();
        m_inProgress = true;
        OnPropertyChanged("InProgress");
        m_monthGroups = ref new Vector<Object^>();

        PhotoReader reader;
        auto foldersTask = task<IVectorView<FolderInformation^>^>(reader.GetVirtualPhotoFoldersByMonth());

        foldersTask.then([this](IVectorView<FolderInformation^>^ folders) 
        {
            auto temp = ref new Vector<Object^>();
            std::for_each(begin(folders), end(folders), [this, temp](FolderInformation^ folder) 
            {
                auto photoGroup = ref new MonthGroup(folder, m_photoCache, m_exceptionPolicy);
                temp->Append(photoGroup);
            });
            return temp;
        }, token).then([this](Vector<Object^>^ folders)
        {
            Array<Object^>^ many = ref new Array<Object^>(folders->Size);
            folders->GetMany(0, many);
            m_monthGroups->ReplaceAll(many);
			OnPropertyChanged("MonthGroups");
        }, token, task_continuation_context::use_current()).then([this, token](task<void> priorTask)
        {
            if (token.is_canceled())
            {
                // Didn't load the photos so reset the underlying items.
                m_monthGroups = nullptr;
            }

            m_inProgress = false;
            OnPropertyChanged("InProgress");
        }, task_continuation_context::use_current()).then(ObserveException<void>(m_exceptionPolicy));
    }
    return m_monthGroups;
}

Photo^ ImageBrowserViewModel::GetPhotoForYearAndMonth(unsigned int year, unsigned int month)
{
    return m_photoCache->GetForYearAndMonth(year, month);
}

void ImageBrowserViewModel::OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
    m_cancelTokenSource.cancel();
}

Object^ ImageBrowserViewModel::YearGroups::get()
{
    if (nullptr == m_yearGroups)
    {
        cancellation_token token = m_cancelTokenSource.get_token();
        m_yearGroups = ref new Vector<Object^>();
        PhotoReader reader;
        auto foldersTask = task<IVectorView<FolderInformation^>^>(reader.GetVirtualPhotoFoldersByYear());

        foldersTask.then([this](IVectorView<FolderInformation^>^ folders) 
        {
            auto temp = ref new Vector<Object^>();
            std::for_each(begin(folders), end(folders), [this, temp](FolderInformation^ folder) 
            {
                auto photoGroup = ref new YearGroup(folder, m_exceptionPolicy);
                temp->Append(photoGroup);
            });
            return temp;
        }, token).then([this](Vector<Object^>^ folders)
        {
            Array<Object^>^ many = ref new Array<Object^>(folders->Size);
            folders->GetMany(0, many);
            m_yearGroups->ReplaceAll(many);
			OnPropertyChanged("YearGroups");
        }, token, task_continuation_context::use_current()).then([this, token](task<void> priorTask)
        {
            if (token.is_canceled())
            {
                // Didn't load the photos so reset the underlying items.
                m_yearGroups = nullptr;
            }

            m_inProgress = false;
            OnPropertyChanged("InProgress");
        }, task_continuation_context::use_current()).then(ObserveException<void>(m_exceptionPolicy));
    }
    return m_yearGroups;
}

void ImageBrowserViewModel::NavigateToGroup(Object^ parameter)
{
    auto group = dynamic_cast<IPhotoGroup^>(parameter);
    assert(group != nullptr);
    auto photo = dynamic_cast<Photo^>(group->Items->GetAt(0));
    assert(photo != nullptr);
    auto imageData = ref new ImageViewData(photo);
    ViewModelBase::GoToPage(PageType::Image, imageData);
}

bool ImageBrowserViewModel::CanCropOrRotateImage(Object^ paratmer)
{
    return (nullptr != m_photo);
}

void ImageBrowserViewModel::CropImage(Object^ parameter)
{
    FileInformation^ file = m_photo;
    ViewModelBase::GoToPage(PageType::Crop, file);
}

void ImageBrowserViewModel::RotateImage(Object^ parameter)
{
    FileInformation^ file = m_photo;
    ViewModelBase::GoToPage(PageType::Rotate, file);
}

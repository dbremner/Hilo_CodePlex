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
#include "DelegateCommand.h"
#include "PageType.h"
#include "MainHubViewModel.h"
#include "HubGroupType.h"
#include "ImageViewData.h"
#include "Photo.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;

MainHubViewModel::MainHubViewModel(IObservableVector<HubPhotoGroup^>^ photoGroups, IExceptionPolicy^ exceptionPolicy) 
    : m_photoGroups(photoGroups), m_isAppBarEnabled(false), m_photo(nullptr), ViewModelBase(exceptionPolicy)
{
    m_navigateToPicturesCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &MainHubViewModel::NavigateToPictures), ref new CanExecuteDelegate(this, &MainHubViewModel::CanNavigateToPictures));
    m_cropImageCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &MainHubViewModel::CropImage), ref new CanExecuteDelegate(this, &MainHubViewModel::CanCropOrRotateImage));
    m_rotateImageCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &MainHubViewModel::RotateImage), ref new CanExecuteDelegate(this, &MainHubViewModel::CanCropOrRotateImage));

    m_pictureGroup = static_cast<HubPhotoGroup^>(m_photoGroups->GetAt(static_cast<unsigned int>(HubGroupType::Pictures)));
    assert(m_pictureGroup != nullptr);
    m_pictureGroup->PropertyChanged += ref new PropertyChangedEventHandler([this](Object^ sender, PropertyChangedEventArgs^ e) 
    {
        if (e->PropertyName == "Items")
        {
            m_navigateToPicturesCommand->CanExecute(nullptr);
        }
    });
}

Object^ MainHubViewModel::PhotoGroups::get()
{
    return m_photoGroups;
}

bool MainHubViewModel::IsAppBarEnabled::get()
{
    return m_isAppBarEnabled;
}

void MainHubViewModel::IsAppBarEnabled::set(bool value)
{
    if (m_isAppBarEnabled != value)
    {
        m_isAppBarEnabled = value;
        OnPropertyChanged("IsAppBarEnabled");
    }
}

Photo^ MainHubViewModel::SelectedItem::get()
{
    return m_photo;
}

void MainHubViewModel::SelectedItem::set(Photo^ value)
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

ICommand^ MainHubViewModel::NavigateToPicturesCommand::get()
{
    return m_navigateToPicturesCommand;
}

ICommand^ MainHubViewModel::CropImageCommand::get()
{
    return m_cropImageCommand;
}

ICommand^ MainHubViewModel::RotateImageCommand::get()
{
    return m_rotateImageCommand;
}

void MainHubViewModel::NavigateToImageView(Photo^ photo)
{
    if (nullptr != photo)
    {
        auto data = ref new ImageViewData(photo);
        ViewModelBase::GoToPage(PageType::Browse, nullptr); 
    }
}

void MainHubViewModel::NavigateToPictures(Object^ parameter)
{
    ViewModelBase::GoToPage(PageType::Browse, nullptr); 
}

bool MainHubViewModel::CanNavigateToPictures(Object^ parameter)
{
    return (m_pictureGroup->Items->Size > 0);
}

void MainHubViewModel::CropImage(Object^ parameter)
{
    FileInformation^ fileInfo = m_photo;
    ViewModelBase::GoToPage(PageType::Crop, fileInfo);
}

void MainHubViewModel::RotateImage(Object^ parameter)
{
    FileInformation^ fileInfo = m_photo;
    ViewModelBase::GoToPage(PageType::Rotate, fileInfo);
}

bool MainHubViewModel::CanCropOrRotateImage(Object^ paratmer)
{
    return (nullptr != m_photo);
}
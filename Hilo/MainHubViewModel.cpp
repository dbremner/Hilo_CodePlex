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
#include "ImageNavigationData.h"
#include "IPhoto.h"

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

IObservableVector<HubPhotoGroup^>^ MainHubViewModel::PhotoGroups::get()
{
    return m_photoGroups;
}

Object^ MainHubViewModel::SelectedItem::get()
{
    return m_photo;
}

void MainHubViewModel::SelectedItem::set(Object^ value)
{
    if (m_photo != value)
    {
        m_photo = dynamic_cast<IPhoto^>(value);
        m_cropImageCommand->CanExecute(nullptr);
        m_rotateImageCommand->CanExecute(nullptr);
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
    auto data = ref new ImageNavigationData(m_photo);
    ViewModelBase::GoToPage(PageType::Crop, data->SerializeToString());
}

void MainHubViewModel::RotateImage(Object^ parameter)
{
    auto data = ref new ImageNavigationData(m_photo);
    ViewModelBase::GoToPage(PageType::Rotate, data->SerializeToString());
}

bool MainHubViewModel::CanCropOrRotateImage(Object^ paratmer)
{
    return (nullptr != m_photo);
}
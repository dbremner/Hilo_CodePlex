//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ImageViewData.h"
#include "ImageViewModel.h"
#include "DelegateCommand.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;

ImageViewModel::ImageViewModel()
{
    m_editImageCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &ImageViewModel::EditImage), nullptr);
}

ICommand^ ImageViewModel::EditImageCommand::get()
{
    return m_editImageCommand;
}

Object^ ImageViewModel::Photos::get()
{
    return m_photos;
}

ImageSource^ ImageViewModel::Photo::get()
{
    if (m_image == nullptr && m_photo != nullptr)
    {
        auto fileOpenTask = task<IRandomAccessStreamWithContentType^>(m_photo->OpenReadAsync());
        fileOpenTask.then([this](IRandomAccessStreamWithContentType^ imageData) { 
            m_image = ref new BitmapImage();
            m_image->SetSource(imageData);
            //delete imageData;
        }).then([this] {
            OnPropertyChanged("Photo");
        },task_continuation_context::use_current());
    }
    return m_image;
}

FileInformation^ ImageViewModel::SelectedItem::get()
{
    return m_photo;
}

void ImageViewModel::SelectedItem::set(FileInformation^ value)
{
    if (m_photo != value)
    {
        m_photo = value;
        m_image = nullptr;
        OnPropertyChanged("Photo");
    }
}

void ImageViewModel::OnNavigatedTo(NavigationEventArgs^ e)
{
    auto imageViewData = dynamic_cast<ImageViewData^>(e->Parameter);
    if (imageViewData != nullptr)
    {
        m_photo = imageViewData->Photo; 
        m_photos = imageViewData->Photos;
        m_image = nullptr;
    }
}

void ImageViewModel::EditImage(Object^ parameter)
{
    auto storageFile = dynamic_cast<StorageFile^>(m_photo);
    ViewModelBase::GoToPage(PageType::Edit, storageFile);
}
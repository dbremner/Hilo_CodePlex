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
#include "PhotoReader.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Search;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;

ImageViewModel::ImageViewModel() 
    : m_head([]{})
{
    m_cropImageCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &ImageViewModel::CropImage), nullptr);
    m_rotateImageCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &ImageViewModel::RotateImage), nullptr);
}

ICommand^ ImageViewModel::CropImageCommand::get()
{
    return m_cropImageCommand;
}

ICommand^ ImageViewModel::RotateImageCommand::get()
{
    return m_rotateImageCommand;
}

Platform::Object^ ImageViewModel::Photos::get()
{
    if (nullptr == m_photos)
    {
        PhotoReader reader;
        m_photos = reader.GetVirtualizedFiles(m_folder, "");
    }
    return m_photos;
}

ImageSource^ ImageViewModel::Photo::get()
{
    if (nullptr == m_image && nullptr != m_photo)
    {
        GetPhotoAsync();
    }
    return m_image;
}

FileInformation^ ImageViewModel::SelectedItem::get()
{
    return m_photo;
}
void ImageViewModel::SelectedItem::set(FileInformation^ value)
{
    if (m_photo != value && value != nullptr)
    {
        m_photo = value;
        m_image = nullptr;
        GetPhotoAsync();
    }
}

void ImageViewModel::GetPhotoAsync()
{
    m_cts.cancel();
    m_cts = cancellation_token_source();
    auto token = m_cts.get_token();

    if (m_photo == nullptr) return;

    auto fileOpenTask = task<IRandomAccessStreamWithContentType^>(m_photo->OpenReadAsync(), token);
    fileOpenTask.then([this](IRandomAccessStreamWithContentType^ imageData) 
    {
        auto image = ref new BitmapImage();
        image->SetSource(imageData);
        {
            critical_section::scoped_lock lock(m_cs);
            m_image = image;
            image = nullptr;
        }
    }).then([this] 
    {
        OnPropertyChanged("Photo");
    },task_continuation_context::use_current());
}

void ImageViewModel::OnNavigatedTo(NavigationEventArgs^ e)
{
    if (NavigationMode::Back == e->NavigationMode)
    {
        return;
    }

	Initialize(e->Parameter);
}

void ImageViewModel::Initialize(Object^ parameter)
{
	// we came from an edit
    auto newFile = dynamic_cast<FileInformation^>(parameter);
    if (nullptr != newFile)
    {
        m_photo = newFile;
        m_image = nullptr;
    }
    // we came from the image browser
    else
    {
        auto imageViewData = dynamic_cast<ImageViewData^>(parameter);
        if (nullptr != imageViewData)
        {
            m_photo = dynamic_cast<FileInformation^>(imageViewData->Photo); 
            assert(nullptr != m_photo);
            m_folder = dynamic_cast<IStorageFolderQueryOperations^>(imageViewData->Folder);
            assert(nullptr != m_folder);
            m_image = nullptr;
            m_photos = nullptr;
        }
    }
}

void ImageViewModel::CropImage(Object^ parameter)
{
    ViewModelBase::GoToPage(PageType::Crop, m_photo);
}

void ImageViewModel::RotateImage(Object^ parameter)
{
    ViewModelBase::GoToPage(PageType::Rotate, m_photo);
}
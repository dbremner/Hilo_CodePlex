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
#include "TaskExceptionsExtensions.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Search;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;

ImageViewModel::ImageViewModel(IExceptionPolicy^ exceptionPolicy) : ViewModelBase(exceptionPolicy)
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

IObservableVector<Object^>^ ImageViewModel::Photos::get()
{
    if (m_photos == nullptr)
    {
        m_photos = ref new Vector<Object^>();
        m_cts = cancellation_token_source();
        cancellation_token token = m_cts.get_token();
        PhotoReader reader;
        auto photosTask = task<IVectorView<FileInformation^>^>(reader.GetAllPhotosAsync(m_query));
        photosTask.then([this](IVectorView<FileInformation^>^ files)
        {
            auto temp = ref new Vector<Object^>();
            std::for_each(begin(files), end(files), [this, temp](FileInformation^ file) 
            {
                temp->Append(file);
            });
            return temp;
        }, token).then([this](Vector<Object^>^ temp)
        {
            Array<Object^>^ many = ref new Array<Object^>(temp->Size);
            temp->GetMany(0, many);
            m_photos->ReplaceAll(many);
        }, token, task_continuation_context::use_current()).then([this, token](task<void> priorTask)
        {
            if (token.is_canceled())
            {
                // Didn't load the photos so reset the underlying items.
                m_photos = nullptr;
            }

            OnPropertyChanged("Photos");
        }, task_continuation_context::use_current()).then(ObserveException<void>(m_exceptionPolicy));
    }
    return m_photos;
}

FileInformation^ ImageViewModel::SelectedItem::get()
{
    return m_photo;
}

//FileInformation^ ImageViewModel::SelectedItem::get()
//{
//    if (nullptr == m_photo)
//    {
//        GetPhotoAsync().then([this]()
//        {
//            OnPropertyChanged("SelectedItem");
//        }, task_continuation_context::use_current());
//    }
//    return m_photo;
//}

void ImageViewModel::SelectedItem::set(FileInformation^ value)
{
    if (m_photo != value && nullptr != value)
    {
        m_photo = value;
        OnPropertyChanged("SelectedItem");
    }
}

//task<void> ImageViewModel::GetPhotoAsync()
//{
//    m_cts.cancel();
//    m_cts = cancellation_token_source();
//    auto token = m_cts.get_token();
//
//    if (m_filePath == nullptr) return task<void>([]{});
//
//    PhotoReader reader;
//    // "System.FileName:=" + 
//    auto photoTask = reader.GetPhotosAsync(m_filePath, 1);
//    return photoTask.then([this, token](IVectorView<FileInformation^>^ files)
//    {
//        if (files->Size > 0)
//        {
//            m_photo = files->GetAt(0);
//        }
//        else
//        {
//            m_photo = dynamic_cast<FileInformation^>(Photos->GetAt(0));
//        }
//    });
//}

void ImageViewModel::OnNavigatedTo(NavigationEventArgs^ e)
{
    Initialize(e->Parameter);
}

void ImageViewModel::Initialize(Object^ parameter)
{
    // we came from an edit
    auto newFile = dynamic_cast<FileInformation^>(parameter);
    if (nullptr != newFile)
    {
        m_photo = newFile;
    }
    // we came from the image browser
    else
    {
        auto imageViewData = dynamic_cast<ImageViewData^>(parameter);
        if (nullptr != imageViewData)
        {
            m_filePath = imageViewData->FilePath;
            m_query = imageViewData->DateQuery;
            m_photos = nullptr;
            m_photo = nullptr;
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
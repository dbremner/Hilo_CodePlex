//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "DelegateCommand.h"
#include "RotateImageViewModel.h"
#include "PhotoReader.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Streams;
using namespace Windows::Storage::Pickers;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;

using namespace Windows::Storage::Search;
using namespace Windows::Storage::FileProperties;

RotateImageViewModel::RotateImageViewModel() : m_head([](){})
{
    m_rotateCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &RotateImageViewModel::Rotate90), nullptr);
    m_saveCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &RotateImageViewModel::SaveImage), nullptr);
    m_cancelCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &RotateImageViewModel::CancelRotate), nullptr);

    ViewModelBase::m_isAppBarSticky = true;
}

ICommand^ RotateImageViewModel::RotateCommand::get()
{
    return m_rotateCommand;
}

ICommand^ RotateImageViewModel::SaveCommand::get()
{
    return m_saveCommand;
}

ICommand^ RotateImageViewModel::CancelCommand::get()
{
    return m_cancelCommand;
}

bool RotateImageViewModel::IsRotating::get()
{
    return m_isRotating;
}

ImageSource^ RotateImageViewModel::Photo::get()
{
    if (m_image == nullptr && m_file != nullptr)
    {
        auto fileOpenTask = task<IRandomAccessStreamWithContentType^>(m_file->OpenReadAsync());
        fileOpenTask.then([this](IRandomAccessStreamWithContentType^ imageData) 
        {
            m_randomAccessStream = imageData;
            m_image = ref new BitmapImage();
            m_image->SetSource(m_randomAccessStream);
        }).then([this] 
        {
            OnPropertyChanged("Photo");
        },task_continuation_context::use_current());
    }
    return m_image;
}

void RotateImageViewModel::Rotate90(Object^ parameter)
{   
    auto ras = ref new InMemoryRandomAccessStream();
    auto tail = m_head.then([this]()
    {
        m_isRotating = true;
        OnPropertyChanged("IsRotating");
    }, concurrency::task_continuation_context::use_current()).then([this] {
        return BitmapDecoder::CreateAsync(m_randomAccessStream);
    }).then([this, ras](BitmapDecoder^ decoder) 
    {
        return BitmapEncoder::CreateForTranscodingAsync(ras, decoder);
    }).then([](BitmapEncoder^ encoder) 
    {
        encoder->BitmapTransform->Rotation = BitmapRotation::Clockwise90Degrees;
        return encoder->FlushAsync();
    }).then([this, ras]() 
    {
        m_randomAccessStream = ras;
        m_image->SetSource(m_randomAccessStream);
    }).then([this]()
    {
        m_isRotating = false;
        OnPropertyChanged("Photo");
        OnPropertyChanged("IsRotating");
    });
    m_head = tail;
}

void RotateImageViewModel::SaveImage(Object^ parameter)
{
    ImageBase::SaveImage(m_file, m_randomAccessStream);
}

void RotateImageViewModel::CancelRotate(Object^ parameter)
{
    ViewModelBase::GoBack();
}

void RotateImageViewModel::OnNavigatedTo(NavigationEventArgs^ e)
{
    m_file = dynamic_cast<FileInformation^>(e->Parameter);
    m_image = nullptr;
}

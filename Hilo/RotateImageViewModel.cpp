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

double RotateImageViewModel::RotationAngle::get()
{
    return m_rotationAngle;
}

void RotateImageViewModel::RotationAngle::set( double value )
{
    m_rotationAngle = value;
    OnPropertyChanged("RotationAngle");
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
    RotationAngle += 90;
    EndRotation();
}
concurrency::task<void> RotateImageViewModel::DoRotate(double angle)
{
    assert(angle < 360);

    while(angle < 0)
    {
        angle += 360;
    }

    auto ras = ref new InMemoryRandomAccessStream();
    BitmapRotation rotation;

    rotation = static_cast<BitmapRotation>((int)floor(angle / 90));

    return task<BitmapDecoder^>(BitmapDecoder::CreateAsync(m_randomAccessStream))
        .then([this, ras](BitmapDecoder^ decoder) 
    {
        return BitmapEncoder::CreateForTranscodingAsync(ras, decoder);
    }).then([rotation](BitmapEncoder^ encoder) 
    {
        encoder->BitmapTransform->Rotation = rotation;
        return encoder->FlushAsync();
    }).then([this, ras]() 
    {
        m_randomAccessStream = ras;
        m_image->SetSource(m_randomAccessStream);
    }).then([this]()
    {
        RotationAngle = 0;
        OnPropertyChanged("Photo");
    });
}

void RotateImageViewModel::SaveImage(Object^ parameter)
{
    if (m_isSaving) return;

    m_isSaving = true;
    DoRotate(RotationAngle).then([this]()
    {
        ImageBase::SaveImage(m_file, m_randomAccessStream);
    }).then([this](task<void> savedTask)
    {
        try 
        {
            savedTask.get();
        }
        catch(...)
        {
            m_isSaving = false;
            throw;
        }
        m_isSaving = false;
    });
}

void RotateImageViewModel::CancelRotate(Object^ parameter)
{
    ViewModelBase::GoBack();
}

void RotateImageViewModel::OnNavigatedTo(NavigationEventArgs^ e)
{
    Initialize(dynamic_cast<FileInformation^>(e->Parameter));
}

void RotateImageViewModel::Initialize( Windows::Storage::BulkAccess::FileInformation^ image )
{
    m_file = image;
    assert(m_file!= nullptr);    
    m_image = nullptr;
}

void RotateImageViewModel::EndRotation()
{
    auto quarterTurns = (RotationAngle / 90);
    auto nearestQuarter = (int)floor(quarterTurns + 0.5) % 4;
    RotationAngle = (double)nearestQuarter * 90;
}



//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ImageNavigationData.h"
#include "ImageViewModel.h"
#include "IPhoto.h"
#include "IPhotoGroup.h"
#include "NullPhotoGroup.h"
#include "DelegateCommand.h"
#include "SimpleQueryOperation.h"
#include "FileRepository.h"
#include "TaskExceptionsExtensions.h"

using namespace concurrency;
using namespace std;

using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Globalization;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Search;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;

String^ const QueryMapKey = "query";
String^ const FilePathMapKey = "filePath";
String^ const FileDateMapKey = "fileDate";

ImageViewModel::ImageViewModel(IRepository^ repository, IExceptionPolicy^ exceptionPolicy) : ViewModelBase(exceptionPolicy), m_repository(repository), m_runningGetPhotoAsync(false)
{
    m_cropImageCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &ImageViewModel::CropImage), nullptr);
    m_rotateImageCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &ImageViewModel::RotateImage), nullptr);

    m_dataToken = m_repository->DataChanged += ref new DataChangedEventHandler(this, &ImageViewModel::OnDataChanged);
}

ImageViewModel::~ImageViewModel()
{
    m_repository->DataChanged -= m_dataToken;
}

ICommand^ ImageViewModel::CropImageCommand::get()
{
    return m_cropImageCommand;
}

ICommand^ ImageViewModel::RotateImageCommand::get()
{
    return m_rotateImageCommand;
}

IObservableVector<IPhoto^>^ ImageViewModel::Photos::get()
{
    if (m_photos == nullptr && !m_runningGetPhotosAsync)
    {
        OnDataChanged();
    }
    return m_photos;
}

task<void> ImageViewModel::QueryPhotosAsync()
{
    if (m_photos == nullptr)
    {
        m_photos = ref new Vector<IPhoto^, PhotoPathComparer>();
    }

    m_photosCts = cancellation_token_source();
    cancellation_token token = m_photosCts.get_token();
    task<IVectorView<IPhoto^>^> t = create_task(m_repository->GetPhotosForGroupWithQueryOperationAsync(ref new NullPhotoGroup(), ref new SimpleQueryOperation(m_query)), token);
    return t.then([this](IVectorView<IPhoto^>^ photos)
    {
        auto temp = ref new Vector<IPhoto^>();
        for (auto photo : photos)
        {
            temp->Append(photo);
        };
        return temp;
    }, task_continuation_context::use_arbitrary()).then([this](Vector<IPhoto^>^ temp)
    {
        Array<IPhoto^>^ many = ref new Array<IPhoto^>(temp->Size);
        temp->GetMany(0, many);
        m_photos->ReplaceAll(many);
    });
}

Object^ ImageViewModel::SelectedItem::get()
{
    if (nullptr == m_photo && !m_runningGetPhotoAsync)
    {
        m_runningGetPhotoAsync = true;
        QueryPhotoAsync().then([this]()
        {
            OnPropertyChanged("SelectedItem");
            m_runningGetPhotoAsync = false;
        }).then(ObserveException<void>(m_exceptionPolicy));
    }
    return m_photo;
}

void ImageViewModel::SelectedItem::set(Object^ value)
{
    IPhoto^ newPhoto = dynamic_cast<IPhoto^>(value);
    PhotoPathComparer compare;
    bool equal = compare(m_photo, newPhoto);
    if (!equal && nullptr != newPhoto && !m_runningGetPhotoAsync)
    {
        ClearCachedData();
        m_photo = newPhoto;
        OnPropertyChanged("SelectedItem");
    }
}

String^ ImageViewModel::MonthAndYear::get()
{
    Calendar cal;
    cal.SetDateTime(m_fileDate);
    std::wstringstream monthAndYear;
    monthAndYear << cal.MonthAsString()->Data() << " " << cal.YearAsString()->Data();
    return ref new String(monthAndYear.str().c_str());
}

void ImageViewModel::SaveState(IMap<String^, Object^>^ stateMap)
{
    assert(m_photo != nullptr);

    std::wstringstream stringSerialization;
    stringSerialization << m_photo->DateTaken.UniversalTime ;

    stateMap->Insert(FilePathMapKey, m_photo->Path);
    stateMap->Insert(FileDateMapKey, ref new String(stringSerialization.str().c_str()));
    stateMap->Insert(QueryMapKey, m_query);
    return;
}

void ImageViewModel::LoadState(IMap<String^, Object^>^ stateMap)
{
    if (stateMap != nullptr)
    {
        auto filePath = dynamic_cast<String^>(stateMap->Lookup(FilePathMapKey));

        auto fileDateString = dynamic_cast<String^>(stateMap->Lookup(FileDateMapKey));
        DateTime fileDate;
        fileDate.UniversalTime = _wtoi64(fileDateString->Data());

        auto query = dynamic_cast<String^>(stateMap->Lookup(QueryMapKey));

        Initialize(filePath, fileDate, query);
    }
}

task<void> ImageViewModel::QueryPhotoAsync()
{
    m_photoCts.cancel();
    m_photoCts = cancellation_token_source();
    auto token = m_photoCts.get_token();

    if (m_filePath == nullptr) return create_task([]{});

    auto photoTask = create_task(m_repository->GetPhotoForGroupWithQueryOperationAsync(ref new NullPhotoGroup(), ref new SimpleQueryOperation(m_filePath)), token);
    return photoTask.then([this](IPhoto^ photo)
    {
        m_photo = photo;
    }).then([this](task<void> priorTask)
    {
        try
        {
            priorTask.get();
        }
        catch (const concurrency::task_canceled&)
        {
            m_photo = nullptr;
        }
    });
}

void ImageViewModel::OnNavigatedTo(NavigationEventArgs^ e)
{
    auto data = dynamic_cast<String^>(e->Parameter);
    auto imageData = ref new ImageNavigationData(data);
    Initialize(imageData);
}

void ImageViewModel::OnNavigatedFrom(NavigationEventArgs^ e)
{
    m_photoCts.cancel();
    m_photosCts.cancel();
    ClearCachedData();
}

void ImageViewModel::Initialize(ImageNavigationData^ imageNavigationData)
{
    if (nullptr != imageNavigationData)
    {
        Initialize(imageNavigationData->FilePath, imageNavigationData->FileDate, imageNavigationData->DateQuery);        
        m_photos = nullptr;
        m_photo = nullptr;
        m_runningGetPhotoAsync = false;
        m_runningGetPhotosAsync = false;
    }
}

void ImageViewModel::Initialize(String^ filePath, Windows::Foundation::DateTime fileDate, String^ query)
{
    m_filePath = filePath;
    m_fileDate = fileDate;
    m_query = query;
    m_photos = nullptr;
    m_photo = nullptr;
    m_runningGetPhotoAsync = false;
    m_runningGetPhotosAsync = false;
}

void ImageViewModel::CropImage(Object^ parameter)
{
    ImageNavigationData^ data = ref new ImageNavigationData(m_photo);
    ViewModelBase::GoToPage(PageType::Crop, data->SerializeToString());
}

void ImageViewModel::RotateImage(Object^ parameter)
{
    ImageNavigationData^ data = ref new ImageNavigationData(m_photo);
    ViewModelBase::GoToPage(PageType::Rotate, data->SerializeToString());
}

void ImageViewModel::OnDataChanged()
{
    m_runningGetPhotosAsync = true;
    QueryPhotosAsync().then([this](task<void> priorTask)
    {
        try 
        {
            priorTask.get();
        }
        catch(const concurrency::task_canceled &)
        {
            // Didn't load the photos so reset the underlying items.
            m_photos = nullptr;
        }

        m_runningGetPhotosAsync = false;
        OnPropertyChanged("Photos");
        OnPropertyChanged("SelectedItem");
    }).then(ObserveException<void>(m_exceptionPolicy));
}

void ImageViewModel::ClearCachedData()
{
    if (m_photo != nullptr)
    {
        m_photo->ClearImageData();
    }
}
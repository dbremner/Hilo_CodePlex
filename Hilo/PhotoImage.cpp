// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "PhotoImage.h"
#include "Photo.h"
#include "ExceptionPolicy.h"
#include "TaskExceptionsExtensions.h"
#include "CalendarExtensions.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace std;
using namespace Windows::Foundation;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::Storage;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Streams;
using namespace Windows::System::UserProfile;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Media::Imaging;

PhotoImage::PhotoImage(Photo^ photo, std::shared_ptr<ExceptionPolicy> exceptionPolicy) : m_photo(photo), 
    m_exceptionPolicy(exceptionPolicy), m_resolution(L""),  m_fileSize(0ul), m_image(nullptr)
{
    // This class is bound to XAML. It must be accessed from the app's main thread.
    assert(IsMainThread());
    assert(photo != nullptr);

    // Monitor events of underlying photo object in order to pass them through
    m_photoPropertyChangedEventToken = photo->PropertyChanged::add(ref new PropertyChangedEventHandler(this, &PhotoImage::OnPhotoPropertyChanged));

    m_dateTaken.UniversalTime = 0ll;
}

// This destructor might be invoked from any thread and must therefore be thread safe.
PhotoImage::~PhotoImage()
{
    // Unsubscribe from event handlers. 
    auto photo = m_photo;
    auto photoPropertyChangedEventToken = m_photoPropertyChangedEventToken;
    auto image = m_image;
    auto imageFailedEventToken = m_imageFailedEventToken;
    // Switch to the main thread for safe access to UI objects.
    // Use local variables to avoid capturing a reference to the object being destroyed.
    run_async_non_interactive([photo, photoPropertyChangedEventToken, image, imageFailedEventToken]()
    {
        photo->PropertyChanged::remove(photoPropertyChangedEventToken);
        if (nullptr != image && imageFailedEventToken.Value != 0)
        {
            image->ImageFailed::remove(imageFailedEventToken);
        }
    });    
}

void PhotoImage::OnPropertyChanged(String^ propertyName)
{
    assert(IsMainThread());
    PropertyChanged(this, ref new PropertyChangedEventArgs(propertyName));
}

String^ PhotoImage::Name::get()
{
    assert(IsMainThread());
    return m_photo->Name;
}

String^ PhotoImage::Path::get()
{
    assert(IsMainThread());
    return m_photo->Path;
}

String^ PhotoImage::FormattedPath::get()
{
    assert(IsMainThread());
    wstring pathAndFileName = m_photo->Path->Data();
    basic_string<char>::size_type index;
    index = pathAndFileName.rfind('\\');
    wstring path = pathAndFileName.substr(0, index);
    return ref new String(path.c_str());
}

String^ PhotoImage::FileType::get()
{
    assert(IsMainThread());
    return m_photo->File->FileType;
}

String^ PhotoImage::FormattedDateTaken::get()
{
    assert(IsMainThread());
    DateTimeFormatter dtf("shortdate", GlobalizationPreferences::Languages);
    return (m_dateTaken.UniversalTime == 0) ? ref new String(L"") : dtf.Format(m_dateTaken);
}

String^ PhotoImage::FormattedTimeTaken::get()
{
    assert(IsMainThread());
    DateTimeFormatter dtf("shorttime", GlobalizationPreferences::Languages);
    return (m_dateTaken.UniversalTime == 0) ? ref new String(L"") : dtf.Format(m_dateTaken);
}

String^ PhotoImage::Resolution::get()
{
    assert(IsMainThread());
    return m_resolution;
}

uint64 PhotoImage::FileSize::get()
{
    assert(IsMainThread());
    return m_fileSize;
}

String^ PhotoImage::DisplayType::get()
{
    assert(IsMainThread());
    return m_photo->File->DisplayType;
}

BitmapImage^ PhotoImage::Thumbnail::get()
{
    assert(IsMainThread());
    return m_photo->Thumbnail;
}

BitmapImage^ PhotoImage::Image::get()
{
    assert(IsMainThread());
    return m_image;
}

bool PhotoImage::IsInvalidThumbnail::get()
{
    assert(IsMainThread());
    return m_photo->IsInvalidThumbnail;
}

// Wrapper to convert task to IAsyncAction (required for public member function of C++/CX ref class)
// Note: Does not throw exceptions. These are handled internally using the object's exception policy.
IAsyncAction^ PhotoImage::InitializeAsync()
{
    return create_async([this]() { return InitializeAsyncInternal(); });
}

// In parallel, fetch image, thumbnail and displayable properties for this image.
// The completion of the individual results will be signaled with PropertyChange events.
// In addition, this function returns a composite task in order to allow unit tests to block until completion.
// Note: Does not throw exceptions. These are handled internally using the object's exception policy.
task<void> PhotoImage::InitializeAsyncInternal()
{
    return (InitializeImageAsync() && InitializeFilePropertiesAsync());
}

// Initialize the properties that depend upon Windows image and basic file properties.
// This function raises PropertyChanged events for FileSize, Resolution, FormattedDateTaken and FormattedTimeTaken. 
// Note: Does not throw exceptions. These are handled internally using the object's exception policy.
task<void> PhotoImage::InitializeFilePropertiesAsync()
{
   assert(IsMainThread());

   // Create containers to store async results when they become available.
   // (We use containers instead of task return values because multiple results are required for subsequent calculations.)
   auto basicPropertiesPtr = std::make_shared<BasicProperties^>(nullptr);
   auto imagePropertiesPtr = std::make_shared<ImageProperties^>(nullptr);
    
   // Start getting image properties; store the result when available.
   task<void> imagePropertiesTask = create_task(m_photo->File->Properties->GetImagePropertiesAsync()).then([imagePropertiesPtr](task<ImageProperties^> imagePropertiesTask)
   {
       try
       {
           (*imagePropertiesPtr) = imagePropertiesTask.get();
       }
       catch (Exception^)
       {
           (*imagePropertiesPtr) = nullptr;
       }
   });  
    
   // Start getting basic properties; store the result when available.
   task<void> basicPropertiesTask = create_task(m_photo->File->GetBasicPropertiesAsync()).then([basicPropertiesPtr](task<BasicProperties^> basicPropertiesTask)
   {
       try
       {
           (*basicPropertiesPtr) = basicPropertiesTask.get();
       }
       catch (Exception^)
       {
           (*basicPropertiesPtr) = nullptr;
       }
   });

   // When the BasicProperties object is available, set all properties that depend on BasicProperties only
   task<void> propertiesTask1 = basicPropertiesTask.then([basicPropertiesPtr, this]()
   {
       assert(IsMainThread());
       if ((*basicPropertiesPtr) == nullptr) return;
       auto newSize = (*basicPropertiesPtr)->Size;
       if (newSize != m_fileSize)
       {
          m_fileSize = newSize;
          OnPropertyChanged("FileSize");
       }
   }, task_continuation_context::use_current());

   // When the ImageProperties object is available, set all properties that depend on ImageProperties only
   task<void> propertiesTask2 = imagePropertiesTask.then([imagePropertiesPtr, this]()
   {
       assert(IsMainThread());
       if ((*imagePropertiesPtr) == nullptr) return;
       String^ newResolution = (*imagePropertiesPtr)->Width + " x " + (*imagePropertiesPtr)->Height;
       if (newResolution != m_resolution)
       {
          m_resolution = newResolution;
          OnPropertyChanged("Resolution");
       }
   }, task_continuation_context::use_current());

   // When both the BasicProperties and ImageProperties objects are available,
   // set all properties that depend on both BasicProperties and ImageProperties.
   // For display, use the "DateTaken" image property, or if not present, the "DateModified" basic property.
   task<void> propertiesTask3 = (basicPropertiesTask && imagePropertiesTask).then([basicPropertiesPtr, imagePropertiesPtr, this]()
   {
       assert(IsMainThread());
       DateTime newDateTaken = {0ll};
       if ((*imagePropertiesPtr) != nullptr)
       {
           newDateTaken = (*imagePropertiesPtr)->DateTaken;
       }
       if (newDateTaken.UniversalTime == 0 && (*basicPropertiesPtr != nullptr))
       {
           newDateTaken = (*basicPropertiesPtr)->DateModified;
       }
       if (newDateTaken.UniversalTime == 0)
       {
           newDateTaken = CalendarExtensions::GetCurrentDateTime();
       }
       if (newDateTaken.UniversalTime != m_dateTaken.UniversalTime)
       {
           m_dateTaken = newDateTaken;
           OnPropertyChanged("FormattedDateTaken");
           OnPropertyChanged("FormattedTimeTaken");
       }
   }, task_continuation_context::use_current());

   // Return a composite task that completes when all of the subtasks have completed.
   return (propertiesTask1.then(ObserveException<void>(m_exceptionPolicy)) &&
           propertiesTask2.then(ObserveException<void>(m_exceptionPolicy)) && 
           propertiesTask3.then(ObserveException<void>(m_exceptionPolicy)));
}   

// Get the full-sized image.
// The result task raises the PropertyChanged("Image") event.
// Note: Does not throw exceptions. These are handled internally using the object's exception policy.
task<void> PhotoImage::InitializeImageAsync()
{
    assert(IsMainThread());
    auto imageStreamTask = create_task(m_photo->File->OpenReadAsync());
    return imageStreamTask.then([this](task<IRandomAccessStreamWithContentType^> priorTask) -> task<void>
    {
        assert(IsMainThread());
        assert(m_image == nullptr);
        IRandomAccessStreamWithContentType^ imageData = priorTask.get();

        m_image = ref new BitmapImage();
        m_imageFailedEventToken = m_image->ImageFailed::add(ref new ExceptionRoutedEventHandler(this, &PhotoImage::OnImageFailedToOpen));
        OnPropertyChanged("Image");
        return create_task(m_image->SetSourceAsync(imageData));
    }).then([this](task<void> priorTask) {
        assert(IsMainThread());
        try
        {
            priorTask.get();
        }
        catch (Exception^)
        {
           OnImageFailedToOpen(nullptr, nullptr);
        }
    }).then(ObserveException<void>(m_exceptionPolicy));
}

void PhotoImage::OnImageFailedToOpen(Object^ sender, ExceptionRoutedEventArgs^ e)
{
    assert(IsMainThread());

    // Load a default image.
    ClearImageData();
    m_image = ref new BitmapImage(ref new Uri("ms-appx:///Assets/HiloLogo.png"));  
    OnPropertyChanged("Image");
}

void PhotoImage::ClearImageData()
{
    if (nullptr != m_image && m_imageFailedEventToken.Value != 0)
    {
        m_image->ImageFailed::remove(m_imageFailedEventToken);
    }
    m_imageFailedEventToken.Value = 0;
}

IAsyncOperation<IRandomAccessStreamWithContentType^>^ PhotoImage::OpenReadAsync()
{
    return m_photo->File->OpenReadAsync();
}

// This event handler listens to all PropertyChanged events of the underlying Photo objects
// and raises the same events for this object. (We do this because some of the properties
// of this object are pass throughs to the Photo object.)
void PhotoImage::OnPhotoPropertyChanged(Object^ sender, PropertyChangedEventArgs^ e)
{
    OnPropertyChanged(e->PropertyName);
}

//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ImageBase.h"

using namespace concurrency;
using namespace Hilo;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Streams;

ImageBase::ImageBase(IExceptionPolicy^ exceptionPolicy) : ViewModelBase(exceptionPolicy)
{
}

IAsyncOperation<Windows::Storage::StorageFile^>^ ImageBase::GetFileNameFromFileSavePickerAsync(Platform::String^ fileType)
{
    return create_async([this, fileType]{ return GetFileNameFromFileSavePickerAsyncImpl(fileType); });
}

IAsyncAction^ ImageBase::SaveImageAsync(StorageFile^ file, IRandomAccessStream^ ras)
{
    return create_async([this, file, ras]{ return SaveImageAsyncImpl(file, ras); });
}

task<StorageFile^> ImageBase::GetFileNameFromFileSavePickerAsyncImpl(String^ fileType)
{
    auto fileExtension = ref new Vector<String^>();
    fileExtension->Append(fileType);

    auto savePicker = ref new FileSavePicker();
    savePicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;    
    savePicker->FileTypeChoices->Insert("Image", fileExtension);
    savePicker->DefaultFileExtension = fileType;
   
    auto filePickerTask = create_task(savePicker->PickSaveFileAsync());
    return filePickerTask.then([](StorageFile^ file)
    {
        if (file == nullptr)
        {
            cancel_current_task();
        }
        return file;
    });
}

task<void> ImageBase::SaveImageAsyncImpl(StorageFile^ file, IRandomAccessStream^ ras)
{
    std::shared_ptr<IBuffer^> buffer = std::make_shared<IBuffer^>(nullptr);

    IInputStream^ inputStream = ras->GetInputStreamAt(0);
    unsigned int size = static_cast<unsigned int>(ras->Size);
    auto streamReader = ref new DataReader(inputStream);
        
    auto loadStreamTask = create_task(streamReader->LoadAsync(size));
    return loadStreamTask.then([streamReader, buffer, size, file](unsigned int loadedBytes)
    {
        *buffer = streamReader->ReadBuffer(size);
        return FileIO::WriteBufferAsync(file, *buffer);
    }).then([this]() 
    {
        ViewModelBase::GoBack();
    });
}

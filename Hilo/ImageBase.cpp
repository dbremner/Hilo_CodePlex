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
#include "PhotoReader.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Streams;

void ImageBase::SaveImage(FileInformation^ file, IRandomAccessStream^ ras)
{
    auto fileExtension = ref new Vector<String^>();
    fileExtension->Append(file->FileType);

    auto savePicker = ref new FileSavePicker();
    savePicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;    
    savePicker->FileTypeChoices->Insert("Image", fileExtension);
    savePicker->DefaultFileExtension = file->FileType;

    std::shared_ptr<IBuffer^> buffer = std::make_shared<IBuffer^>(nullptr);
    std::shared_ptr<String^> newFileName = std::make_shared<String^>(nullptr);

    IInputStream^ inputStream = ras->GetInputStreamAt(0);
    unsigned int size = static_cast<unsigned int>(ras->Size);
    auto streamReader = ref new DataReader(inputStream);
        
    task<unsigned int> loadStreamTask(streamReader->LoadAsync(size));
    loadStreamTask.then([streamReader, savePicker, buffer, size](unsigned int loadedBytes)
    {
        *buffer = streamReader->ReadBuffer(size);
        return savePicker->PickSaveFileAsync();
    }).then([newFileName](StorageFile^ file) 
    {
        if (file == nullptr)
        {
            cancel_current_task();
        }
        *newFileName = file->Name;
        return file;
    }).then([buffer](StorageFile^ file)
    {
        return FileIO::WriteBufferAsync(file, *buffer);
    }).then([newFileName]() 
    {
        PhotoReader reader;
        return reader.GetPhotosAsync(*newFileName, 1);
    }).then([this](IVectorView<FileInformation^>^ files) 
    {
        FileInformation^ fi;
        if (files->Size > 0)
        {
            fi = files->GetAt(0);
        }

        // todo: this will break the navigation metaphor and we cna't pass data: ViewModelBase::GoToPage(PageType::Image, fi);
        ViewModelBase::GoBack();
    });
}

﻿//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ThumbnailGenerator.h"
#include "TaskExtensions.h"

using namespace concurrency;
using namespace std;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Streams;
using namespace Windows::Storage::FileProperties;

using namespace Hilo;

const unsigned int ThumbnailSize = 270;
const wstring ThumbnailImagePrefix = L"thumbImage_";

task<Platform::Collections::Vector<StorageFile^>^> ThumbnailGenerator::Generate( 
    IVector<StorageFile^>^ files, 
    StorageFolder^ thumbnailsFolder)
{
    vector<task<StorageFile^>> thumbnailTasks;

    unsigned int imageCounter = 0;
    for_each(begin(files), end(files),
        [&imageCounter, thumbnailsFolder, &thumbnailTasks](StorageFile^ imageFile)
    {
        wstringstream localFileName;
        localFileName << ThumbnailImagePrefix << imageCounter++ << ".jpg";

        thumbnailTasks.push_back(
            CreateLocalThumbnailAsync(
            thumbnailsFolder,
            imageFile, 
            ref new String(localFileName.str().c_str()),
            ThumbnailSize));   
    });

    return when_all(begin(thumbnailTasks), end(thumbnailTasks)).then(
        [](vector<StorageFile^> files)
    {
        auto result = ref new Platform::Collections::Vector<StorageFile^>();
        for_each(begin(files), end(files), [result](StorageFile^ file)
        {
            if (file != nullptr)
            {
                result->Append(file);
            }
        });

        return result;
    });
}

task<StorageFile^> ThumbnailGenerator::CreateLocalThumbnailAsync(
    StorageFolder^ folder,
    StorageFile^ imageFile,
    String^ localFileName,
    unsigned int thumbSize)
{
    task<InMemoryRandomAccessStream^> createThumbnail(
        CreateThumbnailFromPictureFileAsync(imageFile, thumbSize));

    return createThumbnail.then([folder, localFileName](
        task<InMemoryRandomAccessStream^> createdThumbnailTask) 
    {
        InMemoryRandomAccessStream^ createdThumbnail;
        try 
        {
            createdThumbnail = createdThumbnailTask.get();
        }
        catch(Exception^)
        {
            // If we have any exceptions we won't return the results
            // of this task, but instead nullptr.  Downstream 
            // tasks will need to account for this.
            return task_from_result<StorageFile^>(nullptr);
        }

        return InternalSaveToFile(folder, createdThumbnail, localFileName);
    });
}

task<StorageFile^> ThumbnailGenerator::InternalSaveToFile(
    StorageFolder^ thumbnailsFolder, 
    InMemoryRandomAccessStream^ stream, 
    Platform::String^ filename)
{
    auto imageFile = make_shared<StorageFile^>(nullptr);
    auto streamReader = ref new DataReader(stream);
    task<unsigned int> loadStreamTask(
        streamReader->LoadAsync(static_cast<unsigned int>(stream->Size)));

    return loadStreamTask.then(
        [thumbnailsFolder, filename](unsigned int loadedBytes)
    {
        return thumbnailsFolder->CreateFileAsync(
            filename, 
            CreationCollisionOption::ReplaceExisting);

    }).then([streamReader, imageFile](StorageFile^ thumbnailDestinationFile) 
    {
        (*imageFile) = thumbnailDestinationFile;
        auto buffer = streamReader->ReadBuffer(
            streamReader->UnconsumedBufferLength);

        return FileIO::WriteBufferAsync(thumbnailDestinationFile, buffer);

    }).then([imageFile]()
    {
        return (*imageFile);
    });
}

task<InMemoryRandomAccessStream^> ThumbnailGenerator::CreateThumbnailFromPictureFileAsync(
    StorageFile^ sourceFile, 
    unsigned int thumbSize)
{
    auto decoder = make_shared<BitmapDecoder^>(nullptr);
    auto pixelProvider = make_shared<PixelDataProvider^>(nullptr);
    auto resizedImageStream = ref new InMemoryRandomAccessStream();
    task<StorageItemThumbnail^> createThumbnail(
        sourceFile->GetThumbnailAsync(
        ThumbnailMode::PicturesView, 
        ThumbnailSize));

    return createThumbnail.then([](StorageItemThumbnail^ thumbnail)
    {
        IRandomAccessStream^ imageFileStream = 
            static_cast<IRandomAccessStream^>(thumbnail);

        return BitmapDecoder::CreateAsync(imageFileStream);

    }).then([decoder](BitmapDecoder^ createdDecoder)
    {
        (*decoder) = createdDecoder;
        return createdDecoder->GetPixelDataAsync( 
            BitmapPixelFormat::Rgba8,
            BitmapAlphaMode::Straight,
            ref new BitmapTransform(),
            ExifOrientationMode::IgnoreExifOrientation,
            ColorManagementMode::ColorManageToSRgb);

    }).then([pixelProvider, resizedImageStream](PixelDataProvider^ provider)
    {
        (*pixelProvider) = provider;
        return BitmapEncoder::CreateAsync(
            BitmapEncoder::JpegEncoderId, 
            resizedImageStream);

    }).then([pixelProvider, decoder](BitmapEncoder^ createdEncoder)
    {
        createdEncoder->SetPixelData( BitmapPixelFormat::Rgba8,
            BitmapAlphaMode::Straight,
            (*decoder)->PixelWidth,
            (*decoder)->PixelHeight,
            (*decoder)->DpiX,
            (*decoder)->DpiY,
            (*pixelProvider)->DetachPixelData());
        return createdEncoder->FlushAsync();

    }).then([resizedImageStream]
    {
        resizedImageStream->Seek(0);
        return resizedImageStream;
    });
}
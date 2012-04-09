//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ThumnailGenerator.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Storage::Streams;
using namespace Windows::Storage::FileProperties;

static Platform::String^ ThumbnailImagePrefix = "thumbImage_";
static Platform::String^ ThumbnailsFolderName = "thumbnails";
static Platform::String^ EXIFOrientationPropertyName = "System.Photo.Orientation";

static unsigned int ThumnailSize = 240;

concurrency::task<Platform::Collections::Vector<StorageFile^>^> ThumnailGenerator::Generate( IVector<StorageFile^>^ files, StorageFolder^ thumbnailsFolder)
{
    std::vector<concurrency::task<StorageFile^>> thumbnailTasks;
    
    unsigned int imageCounter = 0;
    std::for_each(begin(files), end(files),[&imageCounter, thumbnailsFolder, &thumbnailTasks](StorageFile^ imageFile)
    {
        std::wstringstream localFileName;
        localFileName << ThumbnailImagePrefix->Data() << imageCounter++ << ".jpg";

        thumbnailTasks.push_back(
            CreateLocalThumbnailAsync(
            thumbnailsFolder, 
            imageFile, 
            ref new Platform::String(localFileName.str().c_str()), 
            ThumnailSize));   
    });

    return when_all(thumbnailTasks.begin(), thumbnailTasks.end())
        .then([](std::vector<StorageFile^> files)
    {
        return ref new Platform::Collections::Vector<StorageFile^>(files);
    });
}

concurrency::task<StorageFile^> ThumnailGenerator::CreateLocalThumbnailAsync(StorageFolder^ folder, StorageFile^ imageFile, Platform::String^ localFileName, unsigned int thumbSize)
{
    concurrency::task<InMemoryRandomAccessStream^> createThumbnail(CreateThumbnailFromPictureFileAsync(imageFile, thumbSize));

    return createThumbnail.then([folder, localFileName](InMemoryRandomAccessStream^ createdThumbnail) {
        return InternalSaveToFile(folder, createdThumbnail, localFileName);
    });
}

concurrency::task<StorageFile^> ThumnailGenerator::InternalSaveToFile(StorageFolder^ folder, InMemoryRandomAccessStream^ stream, Platform::String^ filename)
{
    auto dataWriter = std::make_shared<DataWriter^>(nullptr);
    auto imageFile = std::make_shared<StorageFile^>(nullptr);
    auto streamReader = ref new DataReader(stream);
    concurrency::task<unsigned int> loadStreamTask(streamReader->LoadAsync(static_cast<unsigned int>(stream->Size)));

    return loadStreamTask.then([folder](unsigned int loadedBytes) {
        return folder->GetFolderAsync(ThumbnailsFolderName);
    }).then([filename](StorageFolder^ folder) {        
        return folder->CreateFileAsync(filename, CreationCollisionOption::ReplaceExisting);
    }).then([imageFile](StorageFile^ thumbnailDestinationFile) {
        (*imageFile) = thumbnailDestinationFile;
        return thumbnailDestinationFile->OpenAsync(FileAccessMode::ReadWrite);
    }).then([streamReader, dataWriter](IRandomAccessStream^ writeStream) {
        (*dataWriter) = ref new DataWriter(writeStream);
        (*dataWriter)->WriteBuffer(streamReader->ReadBuffer(streamReader->UnconsumedBufferLength));
        return (*dataWriter)->StoreAsync();
    }).then([dataWriter](unsigned int writtenTask) {
        return(*dataWriter)->FlushAsync();
    }).then([dataWriter, imageFile](bool flushedTask) {
        (*dataWriter)->DetachStream();
        //(*dataWriter)->Close();
        return (*imageFile);
    });
}

concurrency::task<InMemoryRandomAccessStream^> ThumnailGenerator::CreateThumbnailFromPictureFileAsync(StorageFile^ sourceFile, unsigned int thumbSize)
{
    auto dataReader = std::make_shared<DataReader^>(nullptr);
    auto decoder = std::make_shared<BitmapDecoder^>(nullptr);
    auto pixelProvider = std::make_shared<PixelDataProvider^>(nullptr);

    auto originalImageStream = std::make_shared<IRandomAccessStream^>(nullptr);
    auto resizedImageStream = ref new Windows::Storage::Streams::InMemoryRandomAccessStream();

    auto originalImageWidth = std::make_shared<unsigned int>(0);
    auto originalImageHeight = std::make_shared<unsigned int>(0);
    auto originalOrientation = std::make_shared<unsigned short>(0);

    // Aquire info about the image's dimensions
    task<ImageProperties^> propertiesAquiredTask(sourceFile->Properties->GetImagePropertiesAsync());
    return propertiesAquiredTask.then([originalImageHeight, originalImageWidth](Windows::Storage::FileProperties::ImageProperties^ properties) {

        *originalImageHeight = properties->Height;
        *originalImageWidth = properties->Width;

        auto requestedProperties = ref new Platform::Collections::Vector<Platform::String^>();
        requestedProperties->Append(EXIFOrientationPropertyName);

        return properties->RetrievePropertiesAsync(requestedProperties);
    }).then([sourceFile, originalOrientation](IMap<String^, Object^>^ properties) {
        // The orientation is an unsigned short
        auto orientationProperty = safe_cast<IPropertyValue^>(properties->Lookup(EXIFOrientationPropertyName));

        if (orientationProperty != nullptr)
        {
            (*originalOrientation) = orientationProperty->GetUInt16();
        }

        return sourceFile->OpenAsync(FileAccessMode::Read);
    }).then([originalImageStream](IRandomAccessStream^ imageFileStream) {
        // Decode the image
        (*originalImageStream) = imageFileStream;
        return Windows::Graphics::Imaging::BitmapDecoder::CreateAsync(imageFileStream);
    }).then([decoder](Windows::Graphics::Imaging::BitmapDecoder^ createdDecoder) {

        // Create an encoder from the decoder holding the image
        (*decoder) = createdDecoder;

        return createdDecoder->GetPixelDataAsync(   
            BitmapPixelFormat::Rgba8,
            BitmapAlphaMode::Straight,
            ref new BitmapTransform(),
            ExifOrientationMode::IgnoreExifOrientation,
            ColorManagementMode::ColorManageToSRgb);

    }).then([pixelProvider, resizedImageStream](PixelDataProvider^ provider) {
        (*pixelProvider) = provider;

        return BitmapEncoder::CreateAsync(BitmapEncoder::JpegEncoderId, resizedImageStream);
    }).then([pixelProvider, originalImageWidth, originalImageHeight, thumbSize, decoder, originalOrientation](Windows::Graphics::Imaging::BitmapEncoder^ createdEncoder) {

        switch((*originalOrientation))
        {
        case 5:
            // This is a flipped image
            createdEncoder->BitmapTransform->Rotation = BitmapRotation::Clockwise90Degrees;
            break;
        case 6:
            createdEncoder->BitmapTransform->Rotation = BitmapRotation::Clockwise90Degrees;
            break;
        case 7:
            // This is a flipped image
            createdEncoder->BitmapTransform->Rotation = BitmapRotation::Clockwise270Degrees;
            break;
        case 8:
            createdEncoder->BitmapTransform->Rotation = BitmapRotation::Clockwise270Degrees;
            break;
        }

        // Rescale the image to make the longest side the requested size
        // and save the image to the resulting stream
        float scale = (float)*originalImageWidth / *originalImageHeight;
        unsigned int scaledWidth, scaledHeight;

        if((*originalOrientation) == 0)
        {
            scaledWidth = (unsigned int)std::floor(thumbSize * scale);
            scaledHeight = thumbSize;
        }
        else if (scale > 1)
        {
            scaledWidth = (unsigned int)std::floor(thumbSize * scale);
            scaledHeight = thumbSize;
        }
        else
        {
            scaledWidth  = thumbSize;
            scaledHeight = (unsigned int)std::floor(thumbSize * scale);
        }

        createdEncoder->BitmapTransform->ScaledHeight  = scaledHeight;
        createdEncoder->BitmapTransform->ScaledWidth  = scaledWidth;

        createdEncoder->SetPixelData(BitmapPixelFormat::Rgba8,
            BitmapAlphaMode::Straight,
            (*decoder)->PixelWidth,
            (*decoder)->PixelHeight,
            (*decoder)->DpiX,
            (*decoder)->DpiY,
            (*pixelProvider)->DetachPixelData());

        return createdEncoder->FlushAsync();

    }).then([resizedImageStream](){
        // Rewind the stream.
        resizedImageStream->Seek(0);

        return resizedImageStream;
    });
}
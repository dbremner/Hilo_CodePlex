//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

namespace Hilo
{
    class ExceptionPolicy;

    class ThumbnailGenerator
    {
    public:
        ThumbnailGenerator(std::shared_ptr<ExceptionPolicy> policy);
        concurrency::task<Platform::Collections::Vector<Windows::Storage::StorageFile^>^> Generate(Windows::Foundation::Collections::IVector<Windows::Storage::StorageFile^>^ files, Windows::Storage::StorageFolder^ thumbnailsFolder) ;

    private:
        static concurrency::task<Windows::Storage::Streams::InMemoryRandomAccessStream^> CreateThumbnailFromPictureFileAsync(Windows::Storage::StorageFile^ sourceFile, unsigned int thumbSize);
        static concurrency::task<Windows::Storage::StorageFile^> CreateLocalThumbnailAsync(Windows::Storage::StorageFolder^ folder, Windows::Storage::StorageFile^ imageFile, Platform::String^ localFileName, unsigned int thumbSize, std::shared_ptr<ExceptionPolicy> policy);
        static concurrency::task<Windows::Storage::StorageFile^> InternalSaveToFile(Windows::Storage::StorageFolder^ folder, Windows::Storage::Streams::InMemoryRandomAccessStream^ stream, Platform::String^ filename);

        std::shared_ptr<ExceptionPolicy> m_exceptionPolicy;
    } ; 
}


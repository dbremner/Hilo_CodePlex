//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once
#include <ppltasks.h>

namespace Hilo
{
    interface class IExceptionPolicy;

    class ThumbnailGenerator
    {
    public:
        ThumbnailGenerator(IExceptionPolicy^ policy);
        concurrency::task<Platform::Collections::Vector<Windows::Storage::StorageFile^>^> Generate(Windows::Foundation::Collections::IVector<Windows::Storage::StorageFile^>^ files, Windows::Storage::StorageFolder^ thumbnailsFolder) ;

    private:
        static concurrency::task<Windows::Storage::Streams::InMemoryRandomAccessStream^> CreateThumbnailFromPictureFileAsync(Windows::Storage::StorageFile^ sourceFile, unsigned int thumbSize);
        static concurrency::task<Windows::Storage::StorageFile^> CreateLocalThumbnailAsync(Windows::Storage::StorageFolder^ folder, Windows::Storage::StorageFile^ imageFile, Platform::String^ localFileName, unsigned int thumbSize, IExceptionPolicy^ policy);
        static concurrency::task<Windows::Storage::StorageFile^> InternalSaveToFile(Windows::Storage::StorageFolder^ folder, Windows::Storage::Streams::InMemoryRandomAccessStream^ stream, Platform::String^ filename);

        IExceptionPolicy^ m_exceptionPolicy;
    } ; 
}


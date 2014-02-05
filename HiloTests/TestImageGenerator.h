// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#pragma once

namespace HiloTests 
{
    class TestImageGenerator
    {
    public:
        concurrency::task<Windows::Storage::StorageFile^> CreateTestImageFileFromLocalFolder(
            Platform::String^ fileName, 
            Platform::String^ newName = "test.png");

        concurrency::task<Windows::Foundation::Collections::IVectorView<Windows::Storage::StorageFile^>^> CreateTestImagesFromLocalFolder(
            Platform::String^ fileName, 
            unsigned int count,
            Platform::String^ nameBase = "test",
            Platform::String^ extension = ".png"
            );

        concurrency::task<void> DeleteFilesAsync();
    private:
        std::vector<Windows::Storage::IStorageItem^> m_createdFiles;
    };

}
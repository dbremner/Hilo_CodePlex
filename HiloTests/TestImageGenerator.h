//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

namespace HiloTests {

    class TestImageGenerator
    {
    public:
        concurrency::task<Windows::Storage::BulkAccess::FileInformation^> CreateTestImageFileFromLocalFolder(
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
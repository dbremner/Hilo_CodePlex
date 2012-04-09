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
    class RandomPhotoSelector
    {
    public:

        static concurrency::task<Windows::Foundation::Collections::IVector<Windows::Storage::StorageFile^>^> SelectFilesAsync(Windows::Foundation::Collections::IVectorView<Windows::Storage::StorageFile^>^ photos, unsigned int count );
        static std::vector<unsigned int> CreateRandomizedVector(unsigned int vectorSize, unsigned int sampleSize);

    private:
        
    };
}
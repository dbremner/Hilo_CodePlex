//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "RandomPhotoSelector.h"
#include "PhotoReader.h"
#include <map>
#include <ctime>
#include <ppltasks.h>
#include <collection.h>


using namespace concurrency;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Foundation::Collections;

typedef std::map<unsigned int, bool> RandomMap;

task<IVector<StorageFile^>^> Hilo::RandomPhotoSelector::SelectFilesAsync(IVectorView<StorageFile^>^ photos, unsigned int count )
{
    return task<IVector<StorageFile^>^>(
        [photos, count]() 
    {
        auto selectedImages = RandomPhotoSelector::CreateRandomizedVector(photos->Size, count);

        auto selectedFiles = ref new Platform::Collections::Vector<Windows::Storage::StorageFile^>();

        for (unsigned int imageCounter = 0; imageCounter < count; imageCounter++)
        {
            auto imageFile = photos->GetAt(selectedImages[imageCounter]);
            selectedFiles->Append(imageFile);
        }

        return selectedFiles;
    });
}

std::vector<unsigned int> Hilo::RandomPhotoSelector::CreateRandomizedVector(unsigned int vectorSize, unsigned int sampleSize)
{
    RandomMap numbers;

    std::vector<unsigned int> result;

    if (vectorSize >= sampleSize)
    {
        //TODO: Use std::19973?

        srand((unsigned int)time(NULL));
        while (numbers.size() < sampleSize)
        {
            
            int pickFile = (unsigned int) rand() % vectorSize;

            if (numbers.find(pickFile) == numbers.end())
            {
                numbers.insert(RandomMap::value_type(pickFile, true));
                result.push_back(pickFile);
            }
        }
    }

    return result;
}
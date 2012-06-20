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
#include <map>
#include <random>
#include <ctime>
#include <ppltasks.h>
#include <collection.h>

using namespace concurrency;
using namespace std;
using namespace Windows::Storage;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::Foundation::Collections;
using namespace Hilo;

typedef map<unsigned int, bool> RandomMap;

task<IVector<StorageFile^>^> RandomPhotoSelector::SelectFilesAsync(IVectorView<StorageFile^>^ photos, unsigned int count )
{
    return create_task(
        [photos, count]() -> IVector<StorageFile^>^
    {
        auto selectedImages = RandomPhotoSelector::CreateRandomizedVector(photos->Size, count);

        auto selectedFiles = ref new Platform::Collections::Vector<Windows::Storage::StorageFile^>();

        for (unsigned int imageCounter = 0; imageCounter < selectedImages.size(); imageCounter++)
        {
            auto imageFile = photos->GetAt(selectedImages[imageCounter]);
            selectedFiles->Append(imageFile);
        }

        return selectedFiles;
    });
}

vector<unsigned int> RandomPhotoSelector::CreateRandomizedVector(unsigned int vectorSize, unsigned int sampleSize)
{
    RandomMap numbers;

    vector<unsigned int> result;

    if (vectorSize >= sampleSize)
    {
        mt19937 rand(static_cast<unsigned int>(time(NULL)));
        while (numbers.size() < sampleSize)
        {
            int pickFile = static_cast<unsigned int>(rand() % vectorSize);

            if (numbers.find(pickFile) == numbers.end())
            {
                numbers.insert(RandomMap::value_type(pickFile, true));
                result.push_back(pickFile);
            }
        }
    }
    else
    {
        // Don't select any randomized images, just use all the images, no randomization
        for(unsigned int i = 0; i < vectorSize; i++)
        {
            result.push_back(i);
        }
    }

    return result;
}
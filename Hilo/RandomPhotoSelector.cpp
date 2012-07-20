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
#include <random>
#include <ctime>

using namespace concurrency;
using namespace Hilo;
using namespace std;
using namespace Windows::Storage;
using namespace Windows::Foundation::Collections;

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
    // Holds each random number to ensure that we don't choose the same
    // number more than one time.
    typedef map<unsigned int, bool> RandomMap;
    RandomMap numbers;

    // The resulting set of random numbers.
    vector<unsigned int> result;

    if (vectorSize >= sampleSize)
    {
        // Select the set of random numbers.
        mt19937 rand(static_cast<unsigned int>(time(NULL)));
        while (numbers.size() < sampleSize)
        {
            int pickFile = static_cast<unsigned int>(rand() % vectorSize);

            // Use the number if it's unique.
            if (numbers.find(pickFile) == end(numbers))
            {
                numbers.insert(RandomMap::value_type(pickFile, true));
                result.push_back(pickFile);
            }
        }
    }
    else
    {
        // We don't have enough images to choose a random collection.
        // Just use all images.
        for(unsigned int i = 0; i < vectorSize; i++)
        {
            result.push_back(i);
        }
    }

    return result;
}

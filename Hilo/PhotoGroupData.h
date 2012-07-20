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
    interface class IPhoto;

    class PhotoGroupData
    {
    public:
        PhotoGroupData();
        PhotoGroupData(Windows::Foundation::Collections::IVectorView<IPhoto^>^ photos, unsigned int size);
        PhotoGroupData(const PhotoGroupData& data);

        Windows::Foundation::Collections::IVectorView<IPhoto^>^ GetPhotos() const;
        unsigned int GetSize() const;

    private:
        Windows::Foundation::Collections::IVectorView<IPhoto^>^ m_photos;
        unsigned int m_size;
    };

}
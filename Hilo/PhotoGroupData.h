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

    public ref class PhotoGroupData sealed
    {
    public:
        PhotoGroupData(Windows::Foundation::Collections::IVectorView<IPhoto^>^ photos, unsigned int size);

        property Windows::Foundation::Collections::IVectorView<IPhoto^>^ Photos
        {
            Windows::Foundation::Collections::IVectorView<IPhoto^>^ get();
        }
        
        property unsigned int Size
        {
            unsigned int get();
        }

    private:
        Windows::Foundation::Collections::IVectorView<IPhoto^>^ m_photos;
        unsigned int m_size;
    };

}
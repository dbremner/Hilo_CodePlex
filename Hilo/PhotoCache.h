//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once
#include "IPhotoCache.h"

namespace Hilo
{
    interface class IPhoto;
    
    public ref class PhotoCache sealed : public IPhotoCache
    {
    public:
        PhotoCache();

        virtual void InsertPhoto(IPhoto^ photo);
        virtual IPhoto^ GetForYearAndMonth(int year, int month);

    private:
        typedef std::map<int, Platform::WeakReference> MonthPhoto;
        std::map<int, MonthPhoto> m_photoCache;
    };
}

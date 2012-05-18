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
    ref class Photo;

    public ref class PhotoCache sealed
    {
    public:
        PhotoCache();

        void InsertPhoto(Photo^ photo);
        Photo^ GetForYearAndMonth(int year, int month);

    private:
        typedef std::map<int, Platform::WeakReference> MonthPhoto;
        std::map<int, MonthPhoto> m_photoCache;
    };
}

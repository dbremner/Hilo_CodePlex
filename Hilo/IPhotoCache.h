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

    public interface class IPhotoCache
    {
        void InsertPhoto(IPhoto^ photo);
        IPhoto^ GetForYearAndMonth(int year, int month);
    };
}
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
    interface class IYearGroup;

    public interface class IMonthBlock
    {
        property Platform::String^ Name 
        { 
            Platform::String^ get(); 
        }

        property bool HasPhotos
        { 
            bool get();
        }

        property unsigned int Month
        {
            unsigned int get();
        }

        property IYearGroup^ Group
        {
            IYearGroup^ get();
        }
    };
}
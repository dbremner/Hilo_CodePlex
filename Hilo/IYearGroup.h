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
    interface class IMonthBlock;

    public interface class IYearGroup
    {
        property Platform::String^ Title
        { 
            Platform::String^ get();
        }

        property unsigned int Year
        {
            unsigned int get();
        }

        property Windows::Foundation::Collections::IObservableVector<IMonthBlock^>^ Items
        {
            Windows::Foundation::Collections::IObservableVector<IMonthBlock^>^ get();
        }
    };
}
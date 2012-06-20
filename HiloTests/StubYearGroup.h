//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once
#include "..\Hilo\IYearGroup.h"

namespace HiloTests
{
    interface class Hilo::IMonthBlock;

    ref class StubYearGroup sealed : public Hilo::IYearGroup
    {
    public:
        StubYearGroup(Platform::String^ title, unsigned int year);

        property Platform::String^ Title
        { 
            virtual Platform::String^ get();
        }

        property unsigned int Year
        {
            virtual unsigned int get();
        }

        property Windows::Foundation::Collections::IObservableVector<Hilo::IMonthBlock^>^ Items
        {
            virtual Windows::Foundation::Collections::IObservableVector<Hilo::IMonthBlock^>^ get();
        }

    private:
        Platform::String^ m_title;
        unsigned int m_year;
    };
}
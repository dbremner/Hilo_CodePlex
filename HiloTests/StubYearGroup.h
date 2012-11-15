// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#pragma once
#include "..\Hilo\IYearGroup.h"

namespace HiloTests
{
    interface class Hilo::IMonthBlock;

    ref class StubYearGroup sealed : public Hilo::IYearGroup
    {
    public:
        StubYearGroup(Windows::Foundation::DateTime yearDate);

        property Platform::String^ Title
        { 
            virtual Platform::String^ get();
        }

        property int Year
        {
            virtual int get();
        }

        property Windows::Foundation::Collections::IObservableVector<Hilo::IMonthBlock^>^ Items
        {
            virtual Windows::Foundation::Collections::IObservableVector<Hilo::IMonthBlock^>^ get();
        }

    private:
        Windows::Foundation::DateTime m_yearDate;
    };
}
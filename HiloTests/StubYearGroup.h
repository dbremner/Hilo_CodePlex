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
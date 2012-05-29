//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "Common\BindableBase.h"

namespace Hilo
{
    ref class YearGroup;
    interface class IExceptionPolicy;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class MonthBlock sealed : public BindableBase
    {
    public:
        MonthBlock(YearGroup^ yearGroup, unsigned int month, IExceptionPolicy^ exceptionPolicy);

        property Platform::String^ Name 
        { 
            Platform::String^ get(); 
        }

        property bool Enabled
        { 
            bool get();
        }

        property unsigned int Month
        {
            unsigned int get();
        }

        property YearGroup^ Group
        {
            YearGroup^ get();
        }

    private:
        Platform::WeakReference m_weakYearGroup;
        unsigned int m_month;
        unsigned int m_count;
        bool m_determinedEnabled;
        IExceptionPolicy^ m_exceptionPolicy;

        Platform::String^ BuildDateQuery();
    };
}

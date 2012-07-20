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
#include "IYearGroup.h"

namespace Hilo
{
    interface class IMonthBlock;
    class ExceptionPolicy;
    class MonthBlockQuery;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class YearGroup sealed : public Common::BindableBase, public IYearGroup
    {
    internal:
        YearGroup(Platform::String^ name, std::shared_ptr<MonthBlockQuery> query, std::shared_ptr<ExceptionPolicy> exceptionPolicy);

    public:
        property Platform::String^ Title
        { 
            virtual Platform::String^ get();
        }

        property unsigned int Year
        {
            virtual unsigned int get();
        }

        property Windows::Foundation::Collections::IObservableVector<IMonthBlock^>^ Items
        {
            virtual Windows::Foundation::Collections::IObservableVector<IMonthBlock^>^ get();
        }

    private:
        Platform::String^ m_name;
        std::shared_ptr<MonthBlockQuery> m_query;
        std::shared_ptr<ExceptionPolicy> m_exceptionPolicy;
        unsigned int m_year;
        Platform::Collections::Vector<IMonthBlock^>^ m_months;
    };
}

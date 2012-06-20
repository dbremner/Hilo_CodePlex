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
    interface class IExceptionPolicy;
    interface class IMonthBlock;
    interface class IRepository;
    interface class IQueryOperation;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class YearGroup sealed : public Common::BindableBase, public IYearGroup
    {
    public:
        YearGroup(Platform::String^ name, IRepository^ repository, IQueryOperation^ operation, IExceptionPolicy^ exceptionPolicy);

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
        IRepository^ m_repository;
        IQueryOperation^ m_operation;
        IExceptionPolicy^ m_exceptionPolicy;
        unsigned int m_year;
        Platform::Collections::Vector<IMonthBlock^>^ m_months;
    };
}

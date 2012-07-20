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
#include "IMonthBlock.h"

namespace Hilo
{
    interface class IYearGroup;
    interface class IResourceLoader;
    class ExceptionPolicy;
    class MonthBlockQuery;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class MonthBlock sealed : public Common::BindableBase, public IMonthBlock
    {
    internal:
        MonthBlock(IYearGroup^ yearGroup, unsigned int month, IResourceLoader^ resourceLoader, std::shared_ptr<MonthBlockQuery> query, std::shared_ptr<ExceptionPolicy> exceptionPolicy);
        concurrency::task<void> QueryPhotoCount();

    public:
        property Platform::String^ Name 
        { 
            virtual Platform::String^ get(); 
        }

        property bool HasPhotos
        { 
            virtual bool get();
        }

        property unsigned int Month
        {
            virtual unsigned int get();
        }

        property IYearGroup^ Group
        {
            virtual IYearGroup^ get();
        }

    private:
        Platform::WeakReference m_weakYearGroup;
        unsigned int m_month;
        IResourceLoader^ m_resourceLoader;
        std::shared_ptr<MonthBlockQuery> m_query;
        std::shared_ptr<ExceptionPolicy> m_exceptionPolicy;
        unsigned int m_count;
        bool m_runOperation;
        bool m_runningOperation;

        Platform::String^ BuildDateQuery();
    };
}

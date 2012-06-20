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
    interface class IExceptionPolicy;
    interface class IRepository;
    interface class IQueryOperation;
    interface class IResourceLoader;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class MonthBlock sealed : public Common::BindableBase, public IMonthBlock
    {
    public:
        MonthBlock(IYearGroup^ yearGroup, unsigned int month, IResourceLoader^ resourceLoader, IRepository^ repository, IQueryOperation^ queryOperation, IExceptionPolicy^ exceptionPolicy);

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

    internal:
        concurrency::task<void> QueryPhotoCount();

    private:
        Platform::WeakReference m_weakYearGroup;
        unsigned int m_month;
        IResourceLoader^ m_resourceLoader;
        IRepository^ m_repository;
        IQueryOperation^ m_queryOperation;
        IExceptionPolicy^ m_exceptionPolicy;
        unsigned int m_count;
        bool m_runOperation;
        bool m_runningOperation;

        Platform::String^ BuildDateQuery();
    };
}

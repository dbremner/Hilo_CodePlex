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
#include "PageType.h"

namespace Hilo
{
    interface class IExceptionPolicy;

    public delegate void NavigateEventHandler();
    public delegate void PageNavigateEventHandler(PageType page, Platform::Object^ parameter);

#pragma warning(push)
#pragma warning(disable: 4449)
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class ViewModelBase : public BindableBase
    {
    public:
        ViewModelBase(IExceptionPolicy^ exceptionPolicy);

    public:
        event NavigateEventHandler^ NavigateBack;
        event NavigateEventHandler^ NavigateHome;
        event PageNavigateEventHandler^ NavigateToPage;

        property bool IsAppBarSticky { bool get(); void set(bool value); }

        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e);
        virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e);

    protected:
        virtual void GoBack();
        virtual void GoHome();
        virtual void GoToPage(PageType page, Platform::Object^ parameter);

        bool m_isAppBarSticky;
        Hilo::IExceptionPolicy^ m_exceptionPolicy;
    };
#pragma warning(pop)
}

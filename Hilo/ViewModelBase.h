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
    public ref class ViewModelBase : public Common::BindableBase
    {
    internal:
        ViewModelBase(IExceptionPolicy^ exceptionPolicy);

    public:
        event NavigateEventHandler^ NavigateBack;
        event NavigateEventHandler^ NavigateHome;
        event PageNavigateEventHandler^ NavigateToPage;

        property bool IsAppBarSticky { bool get(); void set(bool value); }
        property bool IsAppBarOpen { bool get(); void set(bool value); }

        virtual void LoadState(Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ stateMap);
        virtual void SaveState(Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ stateMap);

        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e);
        virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e);

    protected:
        virtual void GoBack();
        virtual void GoHome();
        virtual void GoToPage(PageType page, Platform::Object^ parameter);
	
	protected private:
        bool m_isAppBarSticky;
        bool m_isAppBarOpen;
        Hilo::IExceptionPolicy^ m_exceptionPolicy;
    };
#pragma warning(pop)
}


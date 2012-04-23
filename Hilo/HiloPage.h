//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once
#include "PageType.h"
#include "ViewModelBase.h"

namespace Hilo
{
    // Suppress class "not consumable from JavaScript because it's not marked 'sealed'" warning
    // currently emitted despite the WebHostHidden attribute
#pragma warning(push)
#pragma warning(disable: 4449)
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class HiloPage : Windows::UI::Xaml::Controls::Page
    {
    public:
        HiloPage();
        void StartLayoutUpdates(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void StopLayoutUpdates(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void InvalidateVisualState();
        void InvalidateVisualState(Windows::UI::ViewManagement::ApplicationViewState viewState);
        property bool UseFilledStateForNarrowWindow
        {
            bool get();
            void set(bool value);
        }
        property Windows::Foundation::Collections::IObservableMap<Platform::String^, Object^>^ DefaultViewModel
        {
            Windows::Foundation::Collections::IObservableMap<Platform::String^, Object^>^ get();
        }

        // Hilo Page
        static property Windows::UI::Xaml::DependencyProperty^ HiloDataContextProperty
        {
            Windows::UI::Xaml::DependencyProperty^ get();
        }

    protected:
        virtual void GoHome(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        virtual void GoBack(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        virtual Platform::String^ DetermineVisualState(Windows::UI::ViewManagement::ApplicationViewState viewState);

        // Hilo Page
        virtual void NavigateBack();
        virtual void NavigateHome();
        virtual void NavigateToPage(PageType page, Platform::Object^ parameter);        
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
        virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

    private:        
        bool _useFilledStateForNarrowWindow;
        Platform::Collections::Map<Platform::String^, Object^>^ _defaultViewModel;
        Windows::Foundation::EventRegistrationToken _viewStateEventToken;
        Windows::Foundation::EventRegistrationToken _windowSizeEventToken;
        Platform::Collections::Vector<Windows::UI::Xaml::Controls::Control^>^ _layoutAwareControls;
        void ViewStateChanged(Windows::UI::ViewManagement::ApplicationView^ sender, Windows::UI::ViewManagement::ApplicationViewStateChangedEventArgs^ e);
        void WindowSizeChanged(Object^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ e);

        // Hilo Page
        std::map<PageType, Windows::UI::Xaml::Interop::TypeName> m_pages;
        Windows::Foundation::EventRegistrationToken m_navigateBackEventToken;
        Windows::Foundation::EventRegistrationToken m_navigateHomeEventToken;
        Windows::Foundation::EventRegistrationToken m_navigateToPageEventToken;
        static Windows::UI::Xaml::DependencyProperty^ m_hiloDataContextProperty;

        void AttachNavigationHandlers(Hilo::ViewModelBase^ viewModel);
        void DetachNavigationHandlers(Hilo::ViewModelBase^ viewModel);

        static void OnHiloDataContextPropertyChanged(Windows::UI::Xaml::DependencyObject^ element, Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ e);
    };
#pragma warning(pop)
}

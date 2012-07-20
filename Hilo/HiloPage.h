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
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class HiloPage : Windows::UI::Xaml::Controls::Page
    {
    internal:
        HiloPage();

    public:
        // LayoutAwarePage
        void StartLayoutUpdates(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void StopLayoutUpdates(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void InvalidateVisualState();
        static property Windows::UI::Xaml::DependencyProperty^ DefaultViewModelProperty
        {
            Windows::UI::Xaml::DependencyProperty^ get();
        };

        property Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^ DefaultViewModel
        {
            Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^ get();
            void set(Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^ value);
        }

        // Hilo Page
        static property Windows::UI::Xaml::DependencyProperty^ HiloDataContextProperty
        {
            Windows::UI::Xaml::DependencyProperty^ get();
        }

    protected:
        // LayoutAwarePage
        virtual void GoHome(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        virtual void GoBack(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        virtual void GoForward(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        virtual Platform::String^ DetermineVisualState(Windows::UI::ViewManagement::ApplicationViewState viewState);

        // HiloPage was overriding these when inherting from LayoutAwarePage, so fo
        // now we'll use our own implementations that combine LayoutAwarePage and HiloPage
        //virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
        //virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
        //virtual void LoadState(Platform::Object^ navigationParameter,
        //  Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ pageState);
        //virtual void SaveState(Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ pageState);

        // Hilo Page
        virtual void NavigateBack();
        virtual void NavigateHome();
        virtual void NavigateToPage(PageType page, Platform::Object^ parameter);        
        virtual void LoadState(Platform::Object^ navigationParameter,
            Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ pageState);
        virtual void SaveState(Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ pageState);
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
        virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

    internal:
        static property bool IsSuspending;
        static void OnHiloDataContextPropertyChanged(Windows::UI::Xaml::DependencyObject^ element, Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ e);

    private:     
        // LayoutAwarePage
        Platform::String^ _pageKey;
        bool _navigationShortcutsRegistered;
        Windows::Foundation::EventRegistrationToken _windowSizeEventToken,
            _acceleratorKeyEventToken, _pointerPressedEventToken;
        Platform::Collections::Vector<Windows::UI::Xaml::Controls::Control^>^ _layoutAwareControls;
        void WindowSizeChanged(Platform::Object^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ e);
        void CoreDispatcher_AcceleratorKeyActivated(Windows::UI::Core::CoreDispatcher^ sender,
            Windows::UI::Core::AcceleratorKeyEventArgs^ args);
        void CoreWindow_PointerPressed(Windows::UI::Core::CoreWindow^ sender,
            Windows::UI::Core::PointerEventArgs^ args);

        // Hilo Page
        Windows::Foundation::EventRegistrationToken m_navigateBackEventToken;
        Windows::Foundation::EventRegistrationToken m_navigateHomeEventToken;
        Windows::Foundation::EventRegistrationToken m_navigateToPageEventToken;

        void AttachNavigationHandlers(Hilo::ViewModelBase^ viewModel);
        void DetachNavigationHandlers(Hilo::ViewModelBase^ viewModel);

        void OnLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void OnUnloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    };
}

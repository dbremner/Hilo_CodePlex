//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "HiloPage.h"

using namespace Hilo;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;

static TypeName hiloPageTypeName = { Hilo::HiloPage::typeid->FullName, TypeKind::Metadata };
static TypeName objectTypeName = { "Object", TypeKind::Primitive };

DependencyProperty^ HiloPage::m_hiloDataContextProperty =
    DependencyProperty::Register(
    "HiloDataContext",
    objectTypeName, 
    hiloPageTypeName, 
    ref new PropertyMetadata(
    nullptr, 
    ref new PropertyChangedCallback(
    &HiloPage::OnHiloDataContextPropertyChanged)
    ));



void HiloPage::OnHiloDataContextPropertyChanged(Windows::UI::Xaml::DependencyObject^ element, Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ e)
{
    HiloPage^ page = static_cast<HiloPage^>(element);
    ViewModelBase^ oldViewModel = dynamic_cast<ViewModelBase^>(e->OldValue);
    page->DetachNavigationHandlers(oldViewModel);

    ViewModelBase^ newViewModel = dynamic_cast<ViewModelBase^>(e->NewValue);
    page->AttachNavigationHandlers(newViewModel);
}

DependencyProperty^ HiloPage::HiloDataContextProperty::get()
{
    return m_hiloDataContextProperty;
}

HiloPage::HiloPage()
{
    if (Windows::ApplicationModel::DesignMode::DesignModeEnabled) return;

    // Map of pages to PageType enumeration
    TypeName editImageViewType = { "Hilo.EditImageView", TypeKind::Metadata };
    m_pages.insert(std::make_pair(PageType::Edit, editImageViewType));
    TypeName cropImageViewType = { "Hilo.CropImageView", TypeKind::Metadata };
    m_pages.insert(std::make_pair(PageType::Crop, cropImageViewType));
    TypeName rotateImageViewType = { "Hilo.RotateImageView", TypeKind::Metadata };
    m_pages.insert(std::make_pair(PageType::Rotate, rotateImageViewType));
    TypeName imageViewType = { "Hilo.ImageView", TypeKind::Metadata };
    m_pages.insert(std::make_pair(PageType::Image, imageViewType));
    TypeName imageBrowserType =  { "Hilo.ImageBrowserView", TypeKind::Metadata };;
    m_pages.insert(std::make_pair(PageType::Browse, imageBrowserType));
    TypeName mainHubType = { "Hilo.MainHubView", TypeKind::Metadata };
    m_pages.insert(std::make_pair(PageType::Hub, mainHubType));

    // binding for data context changed
    SetBinding(HiloPage::HiloDataContextProperty, ref new Binding());

    // Map application view state to visual state for this page when it is part of the visual tree
    Loaded += ref new RoutedEventHandler(this, &HiloPage::StartLayoutUpdates);
    Unloaded += ref new RoutedEventHandler(this, &HiloPage::StopLayoutUpdates);

    // Establish the default view model as the initial DataContext
    DataContext = _defaultViewModel = ref new Map<String^, Object^>(std::less<String^>());
}

void HiloPage::NavigateBack()
{
    GoBack(nullptr, nullptr);
}

void HiloPage::NavigateHome()
{
    GoHome(nullptr, nullptr);
}

void HiloPage::NavigateToPage(PageType page, Platform::Object^ parameter)
{
    if (Frame != nullptr)
    {
        TypeName source = m_pages[page];
        Frame->Navigate(source, parameter);
    }
}

void HiloPage::OnNavigatedTo(NavigationEventArgs^ e)
{
    ViewModelBase^ viewModel = dynamic_cast<ViewModelBase^>(DataContext);
    if (viewModel != nullptr)
    {
        viewModel->OnNavigatedTo(e);
    }
}

void Hilo::HiloPage::OnNavigatedFrom( Windows::UI::Xaml::Navigation::NavigationEventArgs^ e )
{
    ViewModelBase^ viewModel = dynamic_cast<ViewModelBase^>(DataContext);
    if (viewModel != nullptr)
    {
        // Since we never receive destructs, this is the only place to unsubscribe.
        DetachNavigationHandlers(viewModel);
        viewModel->OnNavigatedFrom(e);
    }
}

/// <summary>
/// Gets an implementation of <see cref="IObservableMap<String, Object>"/> set as the page's
/// default <see cref="DataContext"/>.  This instance can be bound and surfaces property change
/// notifications making it suitable for use as a trivial view model.
/// </summary>
IObservableMap<Platform::String^, Object^>^ HiloPage::DefaultViewModel::get()
{
    return _defaultViewModel;
}

/// <summary>
/// Gets a value indicating whether visual states can be a loose interpretation of the actual
/// application view state.  This is often convenient when a page layout is space constrained.
/// </summary>
/// <remarks>
/// The default value of false indicates that the visual state is identical to the view state,
/// meaning that Filled is only used when another application is snapped.  When set to true
/// FullScreenLandscape is used to indicate that at least 1366 virtual pixels of horizontal real
/// estate are available - even if another application is snapped - and Filled indicates a lesser
/// width, even if no other application is snapped.  On a smaller display such as a 1024x768
/// panel this will result in the visual state Filled whenever the device is in landscape
/// orientation.
/// </remarks>
bool HiloPage::UseFilledStateForNarrowWindow::get()
{
    return _useFilledStateForNarrowWindow;
}

/// <summary>
/// Sets a value indicating whether visual states can be a loose interpretation of the actual
/// application view state.  This is often convenient when a page layout is space constrained.
/// </summary>
/// <remarks>
/// The default value of false indicates that the visual state is identical to the view state,
/// meaning that Filled is only used when another application is snapped.  When set to true
/// FullScreenLandscape is used to indicate that at least 1366 virtual pixels of horizontal real
/// estate are available - even if another application is snapped - and Filled indicates a lesser
/// width, even if no other application is snapped.  On a smaller display such as a 1024x768
/// panel this will result in the visual state Filled whenever the device is in landscape
/// orientation.
/// </remarks>
void HiloPage::UseFilledStateForNarrowWindow::set(bool value)
{
    _useFilledStateForNarrowWindow = value;
    this->InvalidateVisualState();
}

/// <summary>
/// Invoked as an event handler to navigate backward in the page's associated <see cref="Frame"/>
/// until it reaches the top of the navigation stack.
/// </summary>
/// <param name="sender">Instance that triggered the event.</param>
/// <param name="e">Event data describing the conditions that led to the event.</param>
void HiloPage::GoHome(Object^ sender, RoutedEventArgs^ e)
{
    // Use the navigation frame to return to the topmost page
    if (Frame != nullptr)
    {
        while (Frame->CanGoBack) Frame->GoBack();
    }
}

/// <summary>
/// Invoked as an event handler to navigate backward in the page's associated <see cref="Frame"/>
/// to go back one step on the navigation stack.
/// </summary>
/// <param name="sender">Instance that triggered the event.</param>
/// <param name="e">Event data describing the conditions that led to the event.</param>
void HiloPage::GoBack(Object^ sender, RoutedEventArgs^ e)
{
    // Use the navigation frame to return to the previous page
    if (Frame != nullptr && Frame->CanGoBack) 
    {
        Frame->GoBack();
    }
}

/// <summary>
/// Invoked as an event handler, typically on the <see cref="Loaded"/> event of a
/// <see cref="Control"/> within the page, to indicate that the sender should start receiving
/// visual state management changes that correspond to application view state changes.
/// </summary>
/// <param name="sender">Instance of <see cref="Control"/> that supports visual state management
/// corresponding to view states.</param>
/// <param name="e">Event data that describes how the request was made.</param>
/// <remarks>The current view state will immediately be used to set the corresponding visual state
/// when layout updates are requested.  A corresponding <see cref="Unloaded"/> event handler
/// connected to <see cref="StopLayoutUpdates"/> is strongly encouraged.  Instances of
/// <see cref="HiloPage"/> automatically invoke these handlers in their Loaded and Unloaded
/// events.</remarks>
/// <seealso cref="DetermineVisualState"/>
/// <seealso cref="InvalidateVisualState"/>
void HiloPage::StartLayoutUpdates(Object^ sender, RoutedEventArgs^ e)
{
    auto control = safe_cast<Control^>(sender);
    if (_layoutAwareControls == nullptr) {
        // Start listening to view state changes when there are controls interested in updates
        _layoutAwareControls = ref new Vector<Control^>();
        _viewStateEventToken = ApplicationView::GetForCurrentView()->ViewStateChanged += ref new TypedEventHandler<ApplicationView^,ApplicationViewStateChangedEventArgs^>(this, &HiloPage::ViewStateChanged);
        _windowSizeEventToken = Window::Current->SizeChanged += ref new WindowSizeChangedEventHandler(this, &HiloPage::WindowSizeChanged);
    }
    _layoutAwareControls->Append(control);

    // Set the initial visual state of the control
    VisualStateManager::GoToState(control, DetermineVisualState(ApplicationView::Value), false);
}

void HiloPage::ViewStateChanged(ApplicationView^ sender, ApplicationViewStateChangedEventArgs^ e)
{
    InvalidateVisualState(e->ViewState);
}

void HiloPage::WindowSizeChanged(Object^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ e)
{
    if (_useFilledStateForNarrowWindow) InvalidateVisualState();
}

/// <summary>
/// Invoked as an event handler, typically on the <see cref="Unloaded"/> event of a
/// <see cref="Control"/>, to indicate that the sender should start receiving visual state
/// management changes that correspond to application view state changes.
/// </summary>
/// <param name="sender">Instance of <see cref="Control"/> that supports visual state management
/// corresponding to view states.</param>
/// <param name="e">Event data that describes how the request was made.</param>
/// <remarks>The current view state will immediately be used to set the corresponding visual state
/// when layout updates are requested.</remarks>
/// <seealso cref="StartLayoutUpdates"/>
void HiloPage::StopLayoutUpdates(Object^ sender, RoutedEventArgs^ e)
{
    auto control = safe_cast<Control^>(sender);
    unsigned int index;
    if (_layoutAwareControls != nullptr && _layoutAwareControls->IndexOf(control, &index))
    {
        _layoutAwareControls->RemoveAt(index);
        if (_layoutAwareControls->Size == 0)
        {
            // Stop listening to view state changes when no controls are interested in updates
            ApplicationView::GetForCurrentView()->ViewStateChanged -= _viewStateEventToken;
            Window::Current->SizeChanged -= _windowSizeEventToken;
            _layoutAwareControls = nullptr;
        }
    }
}

/// <summary>
/// Translates <see cref="ApplicationViewState"/> values into strings for visual state management
/// within the page.  The default implementation uses the names of enum values.  Subclasses may
/// override this method to control the mapping scheme used.
/// </summary>
/// <param name="viewState">View state for which a visual state is desired.</param>
/// <returns>Visual state name used to drive the <see cref="VisualStateManager"/></returns>
/// <seealso cref="InvalidateVisualState"/>
Platform::String^ HiloPage::DetermineVisualState(ApplicationViewState viewState)
{
    auto actualViewState = viewState;
    if (_useFilledStateForNarrowWindow &&
        (viewState == ApplicationViewState::Filled ||
        viewState == ApplicationViewState::FullScreenLandscape))
    {
        // Allow pages to request that the Filled state be used only for landscape layouts narrower
        // than 1366 virtual pixels
        auto windowWidth = Window::Current->Bounds.Width;
        actualViewState = windowWidth >= 1366 ? ApplicationViewState::FullScreenLandscape : ApplicationViewState::Filled;
    }

    switch (actualViewState)
    {
    case ApplicationViewState::Filled: return "Filled";
    case ApplicationViewState::Snapped: return "Snapped";
    case ApplicationViewState::FullScreenPortrait: return "FullScreenPortrait";
    default: case ApplicationViewState::FullScreenLandscape: return "FullScreenLandscape";
    }
}

/// <summary>
/// Updates all controls that are listening for visual state changes with the correct visual
/// state.
/// </summary>
/// <remarks>
/// Typically used in conjunction with overriding <see cref="DetermineVisualState"/> to
/// signal that a different value may be returned even though the view state has not changed.
/// </remarks>
void HiloPage::InvalidateVisualState()
{
    InvalidateVisualState(ApplicationView::Value);
}

/// <summary>
/// Updates all controls that are listening for visual state changes with the correct visual
/// state.
/// </summary>
/// <remarks>
/// Typically used in conjunction with overriding <see cref="DetermineVisualState"/> to
/// signal that a different value may be returned even though the view state has not changed.
/// </remarks>
/// <param name="viewState">The desired view state, or null if the current view state should be
/// used.</param>
void HiloPage::InvalidateVisualState(ApplicationViewState viewState)
{
    if (_layoutAwareControls != nullptr)
    {
        String^ visualState = DetermineVisualState(viewState);
        auto controlIterator = _layoutAwareControls->First();
        while (controlIterator->HasCurrent)
        {
            auto control = controlIterator->Current;
            VisualStateManager::GoToState(control, visualState, false);
            controlIterator->MoveNext();
        }
       /* auto context = DataContext;
        DataContext = nullptr;
        DataContext = context;*/
    }    
}

void HiloPage::AttachNavigationHandlers(ViewModelBase^ viewModel) 
{
    if (nullptr == viewModel) return ;

    // Only resusbcribe if our tokens are empty.
    m_navigateBackEventToken = viewModel->NavigateBack += ref new NavigateEventHandler(this, &HiloPage::NavigateBack);
    m_navigateHomeEventToken = viewModel->NavigateHome += ref new NavigateEventHandler(this, &HiloPage::NavigateHome);
    m_navigateToPageEventToken = viewModel->NavigateToPage += ref new PageNavigateEventHandler(this, &HiloPage::NavigateToPage);
}

void HiloPage::DetachNavigationHandlers(ViewModelBase^ viewModel) 
{
    if (nullptr == viewModel) return ;

    viewModel->NavigateBack -= m_navigateBackEventToken;
    viewModel->NavigateHome -= m_navigateHomeEventToken;
    viewModel->NavigateToPage -= m_navigateToPageEventToken;
}


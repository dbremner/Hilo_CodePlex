//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ViewModelBase.h"

using namespace Hilo;

using namespace Platform;
using namespace Windows::UI::Xaml::Navigation;

ViewModelBase::ViewModelBase(IExceptionPolicy^ exceptionPolicy) : m_exceptionPolicy(exceptionPolicy)
{
}

bool ViewModelBase::IsAppBarSticky::get()
{
    return m_isAppBarSticky;
}

void ViewModelBase::IsAppBarSticky::set(bool value)
{
    if (m_isAppBarSticky != value)
    {
        m_isAppBarSticky = value;
        OnPropertyChanged("IsAppBarSticky");
    }
}

void ViewModelBase::OnNavigatedTo(NavigationEventArgs^ e)
{
}

void ViewModelBase::OnNavigatedFrom(NavigationEventArgs^ e)
{
}

void ViewModelBase::GoBack()
{
     NavigateBack();
}

void ViewModelBase::GoHome()
{
     NavigateHome();
}


void ViewModelBase::GoToPage(PageType page, Object^ parameter)
{
     NavigateToPage(page, parameter);
}
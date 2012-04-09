//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "BindableBase.h"

using namespace Hilo;

using namespace Platform;
using namespace Windows::UI::Xaml::Data;

/// <summary>
/// Notifies listeners that a property value has changed.
/// </summary>
/// <param name="propertyName">Name of the property used to notify listeners.</param>
void BindableBase::OnPropertyChanged(String^ propertyName)
{
    PropertyChanged(this, ref new PropertyChangedEventArgs(propertyName));
}

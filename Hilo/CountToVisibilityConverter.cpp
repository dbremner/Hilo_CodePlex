//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "CountToVisibilityConverter.h"

using namespace Hilo::Converters;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Controls;

Platform::Object^ CountToVisibilityConverter::Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language) 
{
	auto vector = dynamic_cast<IVector<Object^>^>(value);
	
	if (vector == nullptr || vector->Size > 0)
	{
		return ScrollBarVisibility::Auto;
	}

    return ScrollBarVisibility::Hidden;
}

Platform::Object^ CountToVisibilityConverter::ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language)
{
	throw ref new Platform::NotImplementedException();
}


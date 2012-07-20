//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "InverterConverter.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::UI::Xaml::Interop;

Object^ InverterConverter::Convert(Object^ value, TypeName targetType, Object^ parameter, String^)
{
    bool boolValue = safe_cast<bool>(value);

    return !boolValue;
}

Object^ InverterConverter::ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^)
{
    throw ref new NotImplementedException();
}
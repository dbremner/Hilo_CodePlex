//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "BooleanToBrushConverter.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::UI::Xaml::Interop;

Object^ BooleanToBrushConverter::Convert(Object^ value, TypeName targetType, Object^ parameter, String^)
{
    auto boxedBool = dynamic_cast<Box<bool>^>(value);
    auto boolValue = (boxedBool != nullptr && boxedBool->Value);
    return (boolValue ? "#FFF19720" : "#E2E2E2");
}

Object^ BooleanToBrushConverter::ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^)
{
    throw ref new NotImplementedException();
}
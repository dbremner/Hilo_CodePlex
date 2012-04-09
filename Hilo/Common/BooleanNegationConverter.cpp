//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "BooleanNegationConverter.h"

using namespace Hilo;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Interop;

Object^ BooleanNegationConverter::Convert(Object^ value, TypeName targetType, Object^ parameter, String^)
{
    auto boxedBool = dynamic_cast<Box<bool>^>(value);
    auto boolValue = (boxedBool != nullptr && boxedBool->Value);
    return !boolValue;
}

Object^ BooleanNegationConverter::ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^)
{
    auto boxedBool = dynamic_cast<Box<bool>^>(value);
    auto boolValue = (boxedBool != nullptr && boxedBool->Value);
    return !boolValue;
}

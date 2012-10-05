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
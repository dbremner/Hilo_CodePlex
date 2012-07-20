//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "FileSizeConverter.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::UI::Xaml::Interop;

Object^ FileSizeConverter::Convert(Object^ value, TypeName targetType, Object^ parameter, String^)
{
    double size = safe_cast<double>(safe_cast<unsigned long long>(value));
    std::array<String^, 3> units = { "B", "KB", "MB" };
    unsigned int index = 0;
    
    while (size >= 1024)
    {
        size /= 1024;
        index++;
    }
    
    return ToTwoDecimalPlaces(size) + " " + units[index];
}

double FileSizeConverter::ToTwoDecimalPlaces(double value)
{
    double f, intpart, fractpart;
    fractpart = modf(value, &intpart);
    f = floor(fractpart * 100 + 0.5) / 100.0;
    return intpart + f;
}

Object^ FileSizeConverter::ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^)
{
    throw ref new NotImplementedException();
}

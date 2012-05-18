//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "DateTimeToStringConverter.h"

using namespace Hilo::Converters;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::System::UserProfile;

Object^ DateTimeToStringConverter::Convert(Object^ value, TypeName targetType, Object^ parameter, String^ culture)
{
    if (value != nullptr)
    {
        DateTime dateTime = safe_cast<DateTime>(value);
        auto dtf = ref new DateTimeFormatter("shortdate", GlobalizationPreferences::Languages);
        return dtf->Format(dateTime);
    }
    return DependencyProperty::UnsetValue;
}


Object^ DateTimeToStringConverter::ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ culture)
{
    throw ref new Platform::NotImplementedException();
}

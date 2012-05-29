//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "FileInformationToToolTipConverter.h"

using namespace Hilo;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::System::UserProfile;

Object^ FileInformationToToolTipConverter::Convert(Object^ value, TypeName targetType, Object^ parameter, String^ culture)
{
    if (value != nullptr)
    {
        auto file = safe_cast<FileInformation^>(value);
        auto param = safe_cast<String^>(parameter);
        auto dtf = ref new DateTimeFormatter("shortdate", GlobalizationPreferences::Languages);

        if (param->Equals("FileName"))
        {
            return file->Name;
        }
        else if (param->Equals("DateTaken"))
        {
            auto dateTime = DateForFileInformation(file);
            return dtf->Format(dateTime);
        }
    }
    return DependencyProperty::UnsetValue;
}

Object^ FileInformationToToolTipConverter::ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ culture)
{
    throw ref new Platform::NotImplementedException();
}

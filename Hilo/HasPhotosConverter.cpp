//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "HasPhotosConverter.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::UI::Xaml::Interop;

Object^ HasPhotosConverter::Convert(Object^ value, TypeName targetType, Object^ parameter, String^)
{
    bool hasPhotos = safe_cast<bool>(value);
    
    return !hasPhotos;
}

Object^ HasPhotosConverter::ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^)
{
    throw ref new NotImplementedException();
}
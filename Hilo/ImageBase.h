//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "ViewModelBase.h"

namespace Hilo
{
#pragma warning(push)
#pragma warning(disable: 4449)
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class ImageBase : public ViewModelBase
    {
    protected:
        void SaveImage(Windows::Storage::BulkAccess::FileInformation^ file, Windows::Storage::Streams::IRandomAccessStream^ ras);
    };
#pragma warning(pop)
}

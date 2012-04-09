﻿//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "HiloPage.h" // Required by generated header
#include "ThumbnailConverter.h" // Required by generated header
#include "ImageBrowserView.g.h"

namespace Hilo
{
    public ref class ImageBrowserView sealed
    {
    public:
        ImageBrowserView();

    private:

        // TODO: Replace event handlers with behaviors once supported in Blend.
        void OnPhotoItemClicked(Platform::Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
    };
}

//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

namespace Hilo
{
    interface class IPhoto;

    struct PhotoPathComparer : public std::equal_to<IPhoto^>
    {
        bool operator()(IPhoto^ left, IPhoto^ right) const;
    };
}
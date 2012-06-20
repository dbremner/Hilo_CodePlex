//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "PhotoPathComparer.h"
#include "IPhoto.h"

using namespace Hilo;

bool PhotoPathComparer::operator()(IPhoto^ left, IPhoto^ right) const
{
    if (nullptr == left && nullptr == right)
    {
        return true;
    }
    if (nullptr == left || nullptr == right)
    {
        return false;
    }
    bool equal = left->Path == right->Path;
    return equal;
}
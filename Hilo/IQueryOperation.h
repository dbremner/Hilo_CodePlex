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
    public interface class IQueryOperation
    {
        property Platform::String^ Query
        {
            Platform::String^ get();
            void set(Platform::String^ value);
        }

        property unsigned int MaxNumberOfItems
        {
            unsigned int get();
            void set(unsigned int value);
        }

        Platform::Object^ GetOperator();
    };
}
//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once
#include "..\Hilo\IQueryOperation.h"

namespace HiloTests
{
    ref class StubQueryOperation sealed : Hilo::IQueryOperation
    {
    public:
        StubQueryOperation(Platform::Object^ operation);

        property Platform::String^ Query
        {
            virtual Platform::String^ get();
            virtual void set(Platform::String^ value);
        }

        property unsigned int MaxNumberOfItems
        {
            virtual unsigned int get();
            virtual void set(unsigned int value);
        }

        virtual Platform::Object^ GetOperator();

    private:
        Platform::Object^ m_operation;
        unsigned int m_maxNumberOfItems;
        Platform::String^ m_query;
    };
}

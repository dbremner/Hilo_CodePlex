﻿//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

namespace HiloTests
{
    class DispatchedEvent
    {
    public:
        DispatchedEvent();

        void set();
        void wait();

    private:
        concurrency::event m_event;
    };

}
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
    public ref class VariableListView sealed : public Windows::UI::Xaml::Controls::ListView
    {
    protected:
        virtual void PrepareContainerForItemOverride(Windows::UI::Xaml::DependencyObject^ element, Platform::Object^ item) override;
    };
}


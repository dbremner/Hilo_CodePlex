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
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class VariableGridView sealed : public Windows::UI::Xaml::Controls::GridView
    {
    protected:
        virtual void PrepareContainerForItemOverride(Windows::UI::Xaml::DependencyObject^ element, Platform::Object^ item) override;
    };
}


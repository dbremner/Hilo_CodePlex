//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "VariableGridView.h"
#include "HubPhoto.h"

using namespace Hilo;

using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

void VariableGridView::PrepareContainerForItemOverride(DependencyObject^ element, Object^ item)
{
    auto model = dynamic_cast<HubPhoto^>(item);

    if (model != nullptr)
    {
        element->SetValue(VariableSizedWrapGrid::ColumnSpanProperty, model->ColumnSpan);
        element->SetValue(VariableSizedWrapGrid::RowSpanProperty, model->RowSpan);
    }

    //GridView::PrepareContainerForItemOverride(element, item);
    ((IItemsControlOverrides^)this->__abi_baseclass)->PrepareContainerForItemOverride(element, item);
}
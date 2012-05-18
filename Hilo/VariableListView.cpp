//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "VariableListView.h"
#include "IResizable.h"

using namespace Hilo;

using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

void VariableListView::PrepareContainerForItemOverride(DependencyObject^ element, Object^ item)
{
    auto model = dynamic_cast<IResizable^>(item);

    if (model != nullptr)
    {
        element->SetValue(VariableSizedWrapGrid::ColumnSpanProperty, model->ColumnSpan);
        element->SetValue(VariableSizedWrapGrid::RowSpanProperty, model->RowSpan);
    }

    //ListView::PrepareContainerForItemOverride(element, item);
    ((IItemsControlOverrides^)this->__abi_baseclass)->PrepareContainerForItemOverride(element, item);
}
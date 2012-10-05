#include "pch.h"
#include "VariableGridView.h"
#include "IResizable.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::UI::Xaml::Controls;

// <snippet920>
void VariableGridView::PrepareContainerForItemOverride(DependencyObject^ element, Object^ item)
{
    auto model = dynamic_cast<IResizable^>(item);

    if (model != nullptr)
    {
        element->SetValue(VariableSizedWrapGrid::ColumnSpanProperty, model->ColumnSpan);
        element->SetValue(VariableSizedWrapGrid::RowSpanProperty, model->RowSpan);
    }

    GridView::PrepareContainerForItemOverride(element, item);
}
// </snippet920>
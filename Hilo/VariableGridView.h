#pragma once

namespace Hilo
{
    // <snippet919>
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class VariableGridView sealed : public Windows::UI::Xaml::Controls::GridView
    {
    protected:
        virtual void PrepareContainerForItemOverride(Windows::UI::Xaml::DependencyObject^ element, Platform::Object^ item) override;
    };
    // </snippet919>
}
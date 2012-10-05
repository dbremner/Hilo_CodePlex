#pragma once

namespace Hilo
{
    // See http://go.microsoft.com/fwlink/?LinkId=267279 for info about data converters in the Hilo app.

    // The BooleanToBrushConverter class converts true/false values to two distinct colors. It is used by XAML 
    // controls to indicate state based on fill color.
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class BooleanToBrushConverter sealed : public Windows::UI::Xaml::Data::IValueConverter
    {
    public:
        virtual Object^ Convert(Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Object^ parameter, Platform::String^ language);
        virtual Object^ ConvertBack(Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Object^ parameter, Platform::String^ language);
    };
}

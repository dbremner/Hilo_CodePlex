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
	namespace Converters
	{
		///<summary>Converts an <see cref="IVector" /> to a <see cref="Visibility.Visible" based on presence of entries in the collection. />
		public ref class CountToVisibilityConverter sealed : Windows::UI::Xaml::Data::IValueConverter
		{
		public:
			virtual Object^ Convert(Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Object^ parameter, Platform::String^);
			virtual Object^ ConvertBack(Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Object^ parameter, Platform::String^);
		};
	}
}
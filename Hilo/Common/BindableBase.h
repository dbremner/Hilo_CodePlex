//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

//#include "pch.h"

namespace Hilo
{
        // Suppress class "not consumable from JavaScript because it's not marked 'sealed'" warning
        // currently emitted despite the WebHostHidden attribute
#pragma warning(push)
#pragma warning(disable: 4449)
        /// <summary>
        /// Implementation of <see cref="INotifyPropertyChanged"/> to simplify models.
        /// </summary>
        [Windows::Foundation::Metadata::WebHostHidden]
        public ref class BindableBase : Windows::UI::Xaml::Data::INotifyPropertyChanged
        {
        public:
            event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;

        protected:
            void OnPropertyChanged(Platform::String^ propertyName);
        };
#pragma warning(pop)
}

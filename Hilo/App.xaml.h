//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

#include "App.g.h"
#include "ViewModelLocator.h" // Required by generated header

namespace Hilo
{
    ref class App sealed
    {
    public:
        App();
        virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ pArgs) override;

    private:
        void OnSuspending(Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
        void OnUnhandledException(_In_ Platform::Object^ sender, _In_ Windows::UI::Xaml::UnhandledExceptionEventArgs^ args);
    };
}

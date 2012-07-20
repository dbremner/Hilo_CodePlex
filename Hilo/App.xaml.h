//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
//
// App.xaml.h
// Declaration of the App class
//

#pragma once

#include "App.g.h"
#include "ViewModelLocator.h"  // Required by generated header

namespace Hilo
{
    class ExceptionPolicy;

    /// <summary>
    /// Provides application-specific behavior to supplement the default Application class.
    /// </summary>
    ref class App sealed
    {
    public:
        App();
        virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ pArgs) override;

    private:
        void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
        std::shared_ptr<ExceptionPolicy> m_exceptionPolicy;
    };
}

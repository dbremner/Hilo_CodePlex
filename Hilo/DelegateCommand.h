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
    public delegate void ExecuteDelegate(Platform::Object^ parameter);
    public delegate bool CanExecuteDelegate(Platform::Object^ parameter);

    public ref class DelegateCommand sealed : public Windows::UI::Xaml::Input::ICommand
    {
    public:
        DelegateCommand(ExecuteDelegate^ execute, CanExecuteDelegate^ canExecute);

        virtual event Windows::Foundation::EventHandler<Platform::Object^>^ CanExecuteChanged;

        virtual void Execute(Platform::Object^ parameter);

        virtual bool CanExecute(Platform::Object^ parameter);

    private:
        ExecuteDelegate^ m_executeDelegate;
        CanExecuteDelegate^ m_canExecuteDelegate;
        bool m_canExecute;
    };
}

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
            DelegateCommand(ExecuteDelegate^ execute, CanExecuteDelegate^ canExecute)
            {
                m_executeDelegate = execute;
                m_canExecuteDelegate = canExecute;
            }

            // TODO: may need to raise this event
            event Windows::Foundation::EventHandler<Platform::Object^>^ CanExecuteChanged;

            void Execute(Platform::Object^ parameter)
            {
                m_executeDelegate(parameter);
            }

            bool CanExecute(Platform::Object^ parameter)
            {
                if (m_canExecuteDelegate == nullptr)
                {
                    return true;
                }
                return m_canExecuteDelegate(parameter);
            }

        private:
            ExecuteDelegate^ m_executeDelegate;
            CanExecuteDelegate^ m_canExecuteDelegate;
        };
}

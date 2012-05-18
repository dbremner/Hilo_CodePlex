//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "DelegateCommand.h"

using namespace Hilo;
using namespace Platform;

DelegateCommand::DelegateCommand(ExecuteDelegate^ execute, CanExecuteDelegate^ canExecute) : m_executeDelegate(execute), m_canExecuteDelegate(canExecute)
{
}

void DelegateCommand::Execute(Object^ parameter)
{
    assert(m_executeDelegate != nullptr);
    if (nullptr != m_executeDelegate)
    {
        m_executeDelegate(parameter);
    }
}

bool DelegateCommand::CanExecute(Object^ parameter)
{
    if (m_canExecuteDelegate == nullptr)
    {
        return true;
    }

    bool temp = m_canExecuteDelegate(parameter);
    if (m_canExecute != temp)
    {
        m_canExecute = temp;
        CanExecuteChanged(this, nullptr);
    }
    return m_canExecute;
}
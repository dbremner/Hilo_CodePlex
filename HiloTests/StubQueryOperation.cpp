//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "StubQueryOperation.h"

using namespace HiloTests;
using namespace Platform;

StubQueryOperation::StubQueryOperation(Object^ operation) : m_operation(operation)
{
}

String^ StubQueryOperation::Query::get()
{
    return m_query;
}

void StubQueryOperation::Query::set(String^ value)
{
    m_query = value;
}

unsigned int StubQueryOperation::MaxNumberOfItems::get()
{
    return m_maxNumberOfItems;
}

void StubQueryOperation::MaxNumberOfItems::set(unsigned int value)
{
    m_maxNumberOfItems = value;
}

Object^ StubQueryOperation::GetOperator()
{
    return m_operation;
}
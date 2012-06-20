//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "SimpleQueryOperation.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::Storage;

SimpleQueryOperation::SimpleQueryOperation(String^ query) : m_query(query), m_maxNumberOfItems(0)
{
}

SimpleQueryOperation::SimpleQueryOperation(Platform::String^ query, unsigned int maxNumberOfItems) : m_query(query), m_maxNumberOfItems(maxNumberOfItems)
{
}

String^ SimpleQueryOperation::Query::get()
{
    return m_query;
}

void SimpleQueryOperation::Query::set(String^ value)
{
    m_query = value;
}

unsigned int SimpleQueryOperation::MaxNumberOfItems::get()
{
    return m_maxNumberOfItems;
}

void SimpleQueryOperation::MaxNumberOfItems::set(unsigned int value)
{
    m_maxNumberOfItems = value;
}

Object^ SimpleQueryOperation::GetOperator()
{
    return nullptr;
}
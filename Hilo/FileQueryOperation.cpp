//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "FileQueryOperation.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::Storage;

FileQueryOperation::FileQueryOperation(IStorageFile^ file) : m_file(file), m_maxNumberOfItems(0)
{
}

String^ FileQueryOperation::Query::get()
{
    return m_query;
}

void FileQueryOperation::Query::set(String^ value)
{
    m_query = value;
}

unsigned int FileQueryOperation::MaxNumberOfItems::get()
{
    return m_maxNumberOfItems;
}

void FileQueryOperation::MaxNumberOfItems::set(unsigned int value)
{
    m_maxNumberOfItems = value;
}

Object^ FileQueryOperation::GetOperator()
{
    return m_file;
}
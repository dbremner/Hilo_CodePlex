//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "FolderQueryOperation.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::Storage::Search;

FolderQueryOperation::FolderQueryOperation(IStorageFolderQueryOperations^ folderQuery) : m_folderQuery(folderQuery), m_maxNumberOfItems(0)
{
}

FolderQueryOperation::FolderQueryOperation(IStorageFolderQueryOperations^ folderQuery, unsigned int maxNumberOfItems) : m_folderQuery(folderQuery), m_maxNumberOfItems(maxNumberOfItems)
{
}

String^ FolderQueryOperation::Query::get()
{
    return m_query;
}

void FolderQueryOperation::Query::set(String^ value)
{
    m_query = value;
}

unsigned int FolderQueryOperation::MaxNumberOfItems::get()
{
    return m_maxNumberOfItems;
}

void FolderQueryOperation::MaxNumberOfItems::set(unsigned int value)
{
    m_maxNumberOfItems = value;
}

Object^ FolderQueryOperation::GetOperator()
{
    return m_folderQuery;
}
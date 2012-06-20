//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once
#include "IQueryOperation.h"

namespace Hilo
{
    public ref class FileQueryOperation sealed : public IQueryOperation
    {
    public:
        FileQueryOperation(Windows::Storage::IStorageFile^ file);

        property Platform::String^ Query
        {
            virtual Platform::String^ get();
            virtual void set(Platform::String^ value);
        }

        property unsigned int MaxNumberOfItems
        {
            virtual unsigned int get();
            virtual void set(unsigned int value);
        }

        virtual Platform::Object^ GetOperator();

    private:
        Windows::Storage::IStorageFile^ m_file;
        unsigned int m_maxNumberOfItems;
        Platform::String^ m_query;
    };
}
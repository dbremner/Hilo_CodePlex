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
    interface class IPhoto;

    public ref class ImageNavigationData sealed
    {
    public:
        ImageNavigationData(IPhoto^ photo);
        ImageNavigationData(Platform::String^ serializationString);

        property Platform::String^ FilePath
        {
            Platform::String^ get();
        }

        property Windows::Foundation::DateTime FileDate
        {
            Windows::Foundation::DateTime get();
        }

        property Platform::String^ DateQuery
        {
            Platform::String^ get();
        }

        Platform::String^ SerializeToString();

    private:
        Windows::Foundation::DateTime m_fileDate;
        Platform::String^ m_dateQuery;
        Platform::String^ m_filePath;
    };
}

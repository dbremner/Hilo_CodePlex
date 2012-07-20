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

    class ImageNavigationData
    {
    public:
        ImageNavigationData(IPhoto^ photo);
        ImageNavigationData(Platform::String^ serializationString);

        Platform::String^ GetFilePath() const;
        Windows::Foundation::DateTime GetFileDate() const;
        Platform::String^ GetDateQuery();
        Platform::String^ SerializeToString();

    private:
        Windows::Foundation::DateTime m_fileDate;
        Platform::String^ m_dateQuery;
        Platform::String^ m_filePath;
    };
}

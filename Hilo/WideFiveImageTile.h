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
    class WideFiveImageTile
    {
    public:
        Windows::UI::Notifications::TileNotification^ GetTileNotification();
        void SetImageFilePaths(const std::vector<std::wstring>& fileNames);
        
    private:
        std::vector<std::wstring> m_fileNames;
        void UpdateContentWithValues(Windows::Data::Xml::Dom::XmlDocument^ content);
    };
}
//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "WideFiveImageTile.h"

using namespace std;
using namespace Platform;
using namespace Windows::UI::Notifications;
using namespace Windows::Data::Xml;
using namespace Windows::Data::Xml::Dom;

using namespace Hilo;

const unsigned int MaxTemplateImages = 5;

TileNotification^ WideFiveImageTile::GetTileNotification()
{
    auto content = TileUpdateManager::GetTemplateContent(
        TileTemplateType::TileWideImageCollection);

    UpdateContentWithValues(content);

    return ref new TileNotification(content);
}

void WideFiveImageTile::SetImageFilePaths(const vector<wstring>& fileNames)
{
    if (fileNames.size() > MaxTemplateImages)
    {
        throw exception("Wide tile can only take up to 5 images.");
    }

    m_fileNames = fileNames;
}

void WideFiveImageTile::UpdateContentWithValues(XmlDocument^ content)
{
    if (m_fileNames.size() == 0) return;

    // Update wide tile template with the selected images.
    for(unsigned int image = 0; image < m_fileNames.size(); image++)
    {
        IXmlNode^ tileImage = content->GetElementsByTagName("image")->GetAt(image);
        tileImage->Attributes->GetNamedItem("src")->InnerText = ref new String(
            m_fileNames[image].c_str());
    }

    TileTemplateType squareTileTemplate = TileTemplateType::TileSquareImage;
    XmlDocument^ squareTileXml = TileUpdateManager::GetTemplateContent(squareTileTemplate);

    IXmlNode^ tileImage = squareTileXml->GetElementsByTagName("image")->First()->Current;
    tileImage->Attributes->GetNamedItem("src")->InnerText = ref new String(
        m_fileNames[0].c_str());

    auto node = content->ImportNode(squareTileXml->GetElementsByTagName("binding")->First()->Current, true);
    content->GetElementsByTagName("visual")->First()->Current->AppendChild(node);
}

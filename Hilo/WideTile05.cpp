//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================

#include "pch.h"
#include "WideTile05.h"

using namespace Hilo;
using namespace Windows::UI::Notifications;
using namespace Windows::Data::Xml;
using namespace Windows::Data::Xml::Dom;

static unsigned int MaxTemplateImages = 5;


Windows::UI::Notifications::TileNotification^ Hilo::WideTile05::GetTileNotification()
{
    auto content = TileUpdateManager::GetTemplateContent(TileTemplateType::TileWideImageCollection);

    UpdateContentWithValues(content);

    return ref new TileNotification(content);
}

void Hilo::WideTile05::SetImageFilePaths( std::vector<std::wstring> fileNames )
{
    if (fileNames.size() > MaxTemplateImages)
    {
        // TODO: pull from resource file.
        throw std::exception("Wide tile can only take up to 5 images.");
    }

    m_fileNames = fileNames;
}

void Hilo::WideTile05::UpdateContentWithValues( XmlDocument^ content )
{
    if (m_fileNames.size() == 0) return;

    // Update Wide tile template with the selected images
    for(unsigned int image = 0; image < m_fileNames.size(); image++)
    {
        IXmlNode^ tileImage = content->GetElementsByTagName("image")->GetAt(image);
        tileImage->Attributes->GetNamedItem("src")->InnerText = ref new Platform::String(m_fileNames[image].c_str());
    }

    TileTemplateType squareTileTemplate = TileTemplateType::TileSquareImage;
    XmlDocument^ squareTileXml = TileUpdateManager::GetTemplateContent(squareTileTemplate);

    IXmlNode^ tileImage = squareTileXml->GetElementsByTagName("image")->First()->Current;
    tileImage->Attributes->GetNamedItem("src")->InnerText = ref new Platform::String(m_fileNames[0].c_str());

    auto node = content->ImportNode(squareTileXml->GetElementsByTagName("binding")->First()->Current, true);
    content->GetElementsByTagName("visual")->First()->Current->AppendChild(node);
}

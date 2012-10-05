#pragma once

namespace Hilo 
{
    interface class IResourceLoader;

    // See http://go.microsoft.com/fwlink/?LinkId=267275 for info about Hilo's implementation of tiles.

    // The WideFieImageTile class is a helper class for managing the app's start tile.
    class WideFiveImageTile
    {
    public:
        WideFiveImageTile();
        WideFiveImageTile(IResourceLoader^ loader);

        Windows::UI::Notifications::TileNotification^ GetTileNotification();
        void SetImageFilePaths(const std::vector<std::wstring>& fileNames);

    private:
        IResourceLoader^ m_loader;
        std::vector<std::wstring> m_fileNames;
        void UpdateContentWithValues(Windows::Data::Xml::Dom::XmlDocument^ content);
    };
}
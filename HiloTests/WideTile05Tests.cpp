//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "CppUnitTest.h"
#include "..\Hilo\WideFiveImageTile.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Hilo;
using namespace Windows::Data::Xml::Dom;
using namespace Windows::UI::Notifications;

namespace HiloTests
{
    TEST_CLASS(WideFiveImageTileTests)
    {
    public:
        TEST_METHOD(WideFiveImageTileTestsReturnsNotificationTileWhenRequested)
        {
            WideFiveImageTile tile;

            auto notification = tile.GetTileNotification();

            Assert::IsNotNull(notification);
        }

        TEST_METHOD(WideFiveImageTileTestsReturnsDefaultContentIfNoFilesProvided)
        {
            WideFiveImageTile tile;
            auto defaultContent = TileUpdateManager::GetTemplateContent(TileTemplateType::TileWideImageCollection);

            auto notification = tile.GetTileNotification();

            Assert::AreEqual(defaultContent->GetXml(), notification->Content->GetXml());
        }

        TEST_METHOD(WideFiveImageTileTestsWhenProvideFileNamesThenReplacesImageSourceInContent)
        {
            WideFiveImageTile tile;

            const std::array<std::wstring,5> names = { L"path1", L"path2", L"path3", L"path4", L"path5" };
            std::vector<std::wstring> fileNames(begin(names), end(names));
            
            tile.SetImageFilePaths(fileNames);

            auto notification = tile.GetTileNotification();
            
            XmlNodeList^ tileImage = notification->Content->GetElementsByTagName("image");
            for(unsigned int i = 0; i < fileNames.size(); i++)
            {
                IXmlNode^ node = tileImage->GetAt(i);
                Platform::String^ sourceValue = node->Attributes->GetNamedItem("src")->InnerText;

                auto original = fileNames[i];
                auto sourceItem = std::wstring(sourceValue->Data());

                Assert::AreEqual(original, sourceItem);
            }
        }

        TEST_METHOD(WideFiveImageTileTestsWhenProvideMoreThanFiveFileNamesThrows)
        {
            WideFiveImageTile tile;

            const std::array<std::wstring,6> names = { L"path1", L"path2", L"path3", L"path4", L"path5", L"pathTooMany" };
            std::vector<std::wstring> fileNames(begin(names), end(names));

            Assert::ExpectException<std::exception>([&tile, fileNames] () {
                tile.SetImageFilePaths(fileNames);
            });
        }

        TEST_METHOD(WideFiveImageTileTestsIncludesTileSquareImageNode)
        {
            WideFiveImageTile tile;

            const std::array<std::wstring,5> names = { L"path1", L"path2", L"path3", L"path4", L"path5" };
            std::vector<std::wstring> fileNames(begin(names), end(names));
            tile.SetImageFilePaths(fileNames);

            auto notification = tile.GetTileNotification();

            auto squareImageBinding = notification->Content->SelectSingleNode("/tile/visual/binding[@template='TileSquareImage']");
            Assert::IsNotNull(squareImageBinding);
        }

        TEST_METHOD(WideFiveImageTileTestsSetsTileSquareImageSourceToFirstFile)
        {
            WideFiveImageTile tile;

            const std::array<std::wstring,5> names = { L"path1", L"path2", L"path3", L"path4", L"path5" };
            std::vector<std::wstring> fileNames(begin(names), end(names));
            tile.SetImageFilePaths(fileNames);

            auto notification = tile.GetTileNotification();

            auto squareImageBinding = notification->Content->SelectSingleNode("/tile/visual/binding[@template='TileSquareImage']");
            auto squareImage = squareImageBinding->SelectSingleNode("image/@src")->InnerText;
            Assert::AreEqual(fileNames[0].c_str(), squareImage->Data());
        }
    };
}
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
#include "..\Hilo\FileInformationToToolTipConverter.h"

using namespace Hilo;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::System::UserProfile;

namespace HiloTests
{
    TEST_CLASS(FileInformationToToolTipConverterTests)
    {
    public:

        TEST_METHOD(FileInformationConverterCanConvertFileName)
        {
            TestImageGenerator imageGenerator;
            String^ value;

            TestHelper::RunUISynced([this, &imageGenerator, &value]()
            {
                auto finalFile = std::make_shared<FileInformation^>(nullptr);
                task_status status;

                auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                    .then([finalFile](FileInformation^ file)
                {
                    (*finalFile) = file;
                });

                TestHelper::RunSynced(t2, status);        
                auto fileInformationConverter = ref new FileInformationToToolTipConverter();
                TypeName fileInformationTypeName = { "FileInformation", TypeKind::Metadata } ;
                value = safe_cast<String^>(fileInformationConverter->Convert(*finalFile, fileInformationTypeName, "FileName", "en-US"));
            });

            Assert::AreEqual(value, "TestFile.png");

            TestHelper::RunUISynced([&imageGenerator]()
            {
                task_status status;
                TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
            });
        }

        // No longer included in tooltip
        //      TEST_METHOD(FileInformationConverterCanConvertDateCreated)
        //{
        //	TestImageGenerator imageGenerator;
        //          String^ value;
        //          DateTime fileDateTime;
        //          
        //          TestHelper::RunUISynced([this, &imageGenerator, &value, &fileDateTime]()
        //          {
        //              auto finalFile = std::make_shared<FileInformation^>(nullptr);
        //              task_status status;

        //              auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
        //                  .then([finalFile](FileInformation^ file)
        //              {
        //                  (*finalFile) = file;
        //              });

        //              TestHelper::RunSynced(t2, status);
        //              fileDateTime = (*finalFile)->DateCreated;
        //	    auto fileInformationConverter = ref new FileInformationToToolTipConverter();
        //              TypeName fileInformationTypeName = { "FileInformation", TypeKind::Metadata } ;
        //              value = safe_cast<String^>(fileInformationConverter->Convert(*finalFile, fileInformationTypeName, "DateCreated", "en-US"));
        //          });

        //          auto dtf = ref new DateTimeFormatter("shortdate", GlobalizationPreferences::Languages);
        //          String^ stringDateTime = dtf->Format(fileDateTime);
        //          
        //          Assert::AreEqual(value, stringDateTime);

        //          TestHelper::RunUISynced([&imageGenerator]()
        //          {
        //              task_status status;
        //              TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
        //          });
        //}

        TEST_METHOD(FileInformationConverterCanConvertDateTaken)
        {
            TestImageGenerator imageGenerator;
            String^ value;
            DateTime fileDateTime;

            TestHelper::RunUISynced([this, &imageGenerator, &value, &fileDateTime]()
            {
                auto finalFile = std::make_shared<FileInformation^>(nullptr);
                task_status status;

                auto t2 = imageGenerator.CreateTestImageFileFromLocalFolder("UnitTestLogo.png", "TestFile.png")
                    .then([finalFile](FileInformation^ file)
                {
                    (*finalFile) = file;
                });

                TestHelper::RunSynced(t2, status);
                fileDateTime = Hilo::DateForFileInformation((*finalFile));
                auto fileInformationConverter = ref new FileInformationToToolTipConverter();
                TypeName fileInformationTypeName = { "FileInformation", TypeKind::Metadata } ;
                value = safe_cast<String^>(fileInformationConverter->Convert(*finalFile, fileInformationTypeName, "DateTaken", "en-US"));
            });

            auto dtf = ref new DateTimeFormatter("shortdate", GlobalizationPreferences::Languages);
            String^ stringDateTime = dtf->Format(fileDateTime);

            Assert::AreEqual(value, stringDateTime);

            TestHelper::RunUISynced([&imageGenerator]()
            {
                task_status status;
                TestHelper::RunSynced(imageGenerator.DeleteFilesAsync(), status);
            });
        }

        TEST_METHOD(FileInformationConverterReturnsUnsetValueIfFileInformationIsNull)
        {
            auto fileInformationConverter = ref new FileInformationToToolTipConverter();
            TypeName fileInformationTypeName = { "FileInformation", TypeKind::Metadata } ;
            auto value = fileInformationConverter->Convert(nullptr, fileInformationTypeName, nullptr, "en-US");

            Assert::AreEqual(value, DependencyProperty::UnsetValue);
        }
    };
}

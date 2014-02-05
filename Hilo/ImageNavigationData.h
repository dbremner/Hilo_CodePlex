// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#pragma once

namespace Hilo
{
    interface class IPhoto;

    // The ImageNavigationData class is a data record that is used for navigating between pages.
    class ImageNavigationData
    {
    public:
        ImageNavigationData::ImageNavigationData(Platform::String^ path, Windows::Foundation::DateTime monthGroupDate);
        ImageNavigationData(Platform::String^ serializationString);

        Platform::String^ GetFilePath() const;
        Windows::Foundation::DateTime GetMonthGroupDate() const;
        Platform::String^ GetDateQuery();
        Platform::String^ SerializeToString();

    private:
        Windows::Foundation::DateTime m_monthGroupDate;
        Platform::String^ m_dateQuery;
        Platform::String^ m_filePath;
    };
}

#pragma once

namespace Hilo
{
    interface class IPhoto;

    // The ImageNavigationData class is a data record that is used for navigating between pages.
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

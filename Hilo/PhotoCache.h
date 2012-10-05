#pragma once

namespace Hilo
{
    interface class IPhoto;
    
    // The PhotoCache class is a helper class for XAML navigation. When the app wants to navigate to a particular
    // month, it uses the first image in that month to locate the correct position in the grid control.
    class PhotoCache
    {
    public:
        void InsertPhoto(IPhoto^ photo);
        IPhoto^ GetForYearAndMonth(int year, int month);
        void Clear();

    private:
        typedef std::map<int, Platform::WeakReference> MonthPhoto;
        std::map<int, MonthPhoto> m_photoCache;
    };
}

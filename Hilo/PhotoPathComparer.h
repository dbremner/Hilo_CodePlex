#pragma once

namespace Hilo
{
    interface class IPhoto;

    // The PhotoPathComparer struct provides a comparison operator for Hilo images.
    struct PhotoPathComparer : public std::equal_to<IPhoto^>
    {
        bool operator()(IPhoto^ left, IPhoto^ right) const;
    };
}
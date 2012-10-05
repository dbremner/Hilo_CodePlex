#include "pch.h"
#include "PhotoPathComparer.h"
#include "IPhoto.h"

using namespace Hilo;

bool PhotoPathComparer::operator()(IPhoto^ left, IPhoto^ right) const
{
    if (nullptr == left && nullptr == right)
    {
        return true;
    }
    if (nullptr == left || nullptr == right)
    {
        return false;
    }
    bool equal = left->Path == right->Path;
    return equal;
}
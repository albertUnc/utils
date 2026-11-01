#pragma once

#include <algorithm>
#include <vector>

namespace albert::list
{
    template <typename T>
    bool isIn(const T &val, const std::vector<T> &list) {
        return std::find(list.begin(), list.end(), val) != list.end();
    }
} // namespace albert::list
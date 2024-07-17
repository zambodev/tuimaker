#ifndef UTILS_HPP
#define UTILS_HPP

#include <tuple>
#include "window.hpp"


namespace UTILS
{
    std::tuple<int, int> getTerminalSize(void);
    int getProgressiveId(void);
    int getWindowDepth(Window* window);
}

#endif

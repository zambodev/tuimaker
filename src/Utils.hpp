#pragma once

#include <tuple>
#include <vector>

namespace tmk
{
    namespace Utils
    {
        int GetTerminalWidth(void);
        int GetTerminalHeight(void);
        int GetProgressiveId(void);
        void EnableBuffInput(void);
        void DisableBuffInput(void);
    }
}

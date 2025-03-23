#pragma once

#include <tuple>
#include <vector>

namespace tmk
{
    namespace Utils
    {
        int get_term_width(void);
        int get_term_height(void);
        int get_progressive_id(void);
        void enable_buff_input(void);
        void disable_buff_input(void);
    }
}

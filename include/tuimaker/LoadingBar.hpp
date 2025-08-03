#pragma once

#include <iostream>
#include <cmath>
#include <tuimaker/Window.hpp>

namespace tmk
{
    class LoadingBar : public Window
    {
    public:
        LoadingBar(WindowSize wsize)
            : Window(wsize),
              perc_(0) {
              };

        ~LoadingBar() {
        };

        void set(uint64_t percentage)
        {
            constexpr uint8_t MAX_DECIMAL = 9;
            constexpr uint8_t MAX_LB_RANGE = 7;
            constexpr uint8_t MIN_LB_RANGE = 1;

            if (percentage > 100)
                return;

            double full_blocks = (((size_.width - 2) / 100.0) * percentage) + 1;
            double dec = (full_blocks - (uint64_t)(full_blocks)) * 10;
            uint64_t inc_block = std::round(1 + ((dec - 1) * (MAX_LB_RANGE - 1) / (MAX_DECIMAL - 1)));
            perc_ = percentage;

            for (uint64_t x = 1; x < (uint64_t)(full_blocks); ++x)
            {
                for (uint64_t y = 1; y < size_.height - 1; ++y)
                {
                    buffer_[y * size_.width + x].character = LB_L_100;
                }
            }

            if (inc_block > 0)
            {
                for (uint64_t y = 1; y < size_.height - 1; ++y)
                {
                    switch (inc_block)
                    {
                    case 1:
                        buffer_[y * size_.width + full_blocks].character = LB_L_12;
                        break;
                    case 2:
                        buffer_[y * size_.width + full_blocks].character = LB_L_25;
                        break;
                    case 3:
                        buffer_[y * size_.width + full_blocks].character = LB_L_37;
                        break;
                    case 4:
                        buffer_[y * size_.width + full_blocks].character = LB_L_50;
                        break;
                    case 5:
                        buffer_[y * size_.width + full_blocks].character = LB_L_62;
                        break;
                    case 6:
                        buffer_[y * size_.width + full_blocks].character = LB_L_75;
                        break;
                    case 7:
                        buffer_[y * size_.width + full_blocks].character = LB_L_87;
                        break;
                    default:
                        buffer_[y * size_.width + full_blocks].character = LB_L_0;
                        break;
                    }
                }
            }
        }

    private:
        enum : wchar_t
        {
            LB_L_0 = TChar::U_SPACE,
            LB_L_12 = u'\u258f',
            LB_L_25 = u'\u258e',
            LB_L_37 = u'\u258d',
            LB_L_50 = u'\u258c',
            LB_L_62 = u'\u258b',
            LB_L_75 = u'\u258a',
            LB_L_87 = u'\u2589',
            LB_L_100 = u'\u2588',
        };

        uint64_t perc_;
    };
}

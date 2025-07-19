#pragma once

#include <tuimaker/Window.hpp>

namespace tmk
{
    class TextBox : public Window
    {
    public:
        TextBox(WindowSize wsize)
            : Window(wsize) {
              };

        ~TextBox() {
        };

        void write(const std::string &&str)
        {
            for (int i = 0; i < str.length(); ++i)
            {
                write_char(str[i]);
            }
        }

        void write_char(const char c)
        {
            static int wordBeginIdx = 0;
            switch (c)
            {
            case ' ':
                wordBeginIdx = 1;
                break;
            case '\n':
                ++cur_y_;
                cur_x_ = 1;
                return;
                break;
            case '\r':
                cur_x_ = 1;
                return;
                break;
            }

            if (cur_y_ >= size_.height - 1)
            {
                for (uint64_t y = 1; y < size_.height - 2; ++y)
                {
                    for (uint64_t x = 1; x < size_.width - 1; ++x)
                    {
                        buffer_[y * size_.width + x] = buffer_[(y + 1) * size_.width + x];
                    }
                }

                cur_y_ = size_.height - 2;
            }

            if (cur_x_ == size_.width - 1)
            {
                int oldWordBeginIdx = wordBeginIdx;
                int cursorTmp = 1;

                for (; wordBeginIdx < 0; ++wordBeginIdx)
                {
                    buffer_[(cur_y_ + 1) * size_.width + cursorTmp] =
                        buffer_[(cur_y_ * size_.width) + cur_x_ + wordBeginIdx];
                    buffer_[(cur_y_ * size_.width) + cur_x_ + wordBeginIdx] = ' ';

                    ++cursorTmp;
                }

                cur_x_ = cursorTmp;
                ++cur_y_;
            }

            buffer_[(cur_y_ * size_.width) + cur_x_] = c;

            --wordBeginIdx;
            ++cur_x_;
        }
    };
}

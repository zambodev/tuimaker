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

        void write_char(char c)
        {
            static int wordBeginIdx = 0;
            switch (c)
            {
            // Backspace
            case 127:
                if (cur_x_ == 1 && cur_y_ == 1)
                {
                    return;
                }
                else if (cur_x_ == 1 && cur_y_ > 1)
                {
                    --cur_y_;
                    cur_x_ = (is_line_empty(cur_y_) ? 1 : get_last_char_idx(cur_y_));

                    return;
                }

                --cur_x_;
                ++wordBeginIdx;

                buffer_[(cur_y_ * size_.width) + cur_x_] = U_SPACE;

                return;
                break;
            case ' ':
                wordBeginIdx = 1;
                c = U_SPACE;
                break;
            case '\n':
                ++cur_y_;
                cur_x_ = 1;
                wordBeginIdx = 1;

                if (cur_y_ >= size_.height - 1)
                    scroll_up();
                return;
                break;
            case '\r':
                cur_x_ = 1;
                return;
                break;
            }

            if (cur_y_ >= size_.height - 1)
                scroll_up();

            if (cur_x_ == size_.width - 1)
            {
                int oldWordBeginIdx = wordBeginIdx;
                int cursorTmp = 1;

                if ((cur_y_ + 1) >= size_.height - 1)
                    scroll_up(false);
                else
                    ++cur_y_;

                for (; wordBeginIdx < 0; ++wordBeginIdx)
                {
                    buffer_[cur_y_ * size_.width + cursorTmp] =
                        buffer_[((cur_y_ - 1) * size_.width) + cur_x_ + wordBeginIdx];
                    buffer_[((cur_y_ - 1) * size_.width) + cur_x_ + wordBeginIdx] = U_SPACE;

                    ++cursorTmp;
                }

                cur_x_ = cursorTmp;
            }

            buffer_[(cur_y_ * size_.width) + cur_x_] = c;

            --wordBeginIdx;
            ++cur_x_;
        }

    private:
        void scroll_up(bool reset_cur_x = true)
        {
            for (uint64_t y = 1; y < size_.height - 2; ++y)
            {
                for (uint64_t x = 1; x < size_.width - 1; ++x)
                {
                    buffer_[y * size_.width + x] = buffer_[(y + 1) * size_.width + x];

                    if (y == (size_.height - 3))
                        buffer_[(y + 1) * size_.width + x] = U_SPACE;
                }
            }

            cur_y_ = size_.height - 2;

            if (reset_cur_x)
                cur_x_ = 1;
        }

        bool is_line_empty(uint64_t line)
        {
            for (uint64_t x = 1; x < size_.width - 1; ++x)
                if (buffer_[line * size_.width + x] != U_SPACE)
                    return false;

            return true;
        }

        uint64_t get_last_char_idx(uint64_t line)
        {
            for (uint64_t x = size_.width - 2; x >= 1; --x)
                if (buffer_[line * size_.width + x] != U_SPACE)
                    return x + 1;

            return 1;
        }
    };
}

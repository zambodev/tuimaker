#pragma once

#include <tuimaker/Window.hpp>

namespace tmk
{
    class TextBox : public Window
    {
    public:
        TextBox(WindowSize wsize)
            : Window(wsize)
        {
            text_buffer_.push_back("");
            it_ = text_buffer_.end() - 1;
        };

        ~TextBox() {
        };

        void write(const std::string &str)
        {
            for (int i = 0; i < str.length(); ++i)
            {
                write_char(str[i]);
            }
        }

        void write_char(char c)
        {
            static uint64_t word_len = 0;

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
                    cur_x_ = (is_line_empty(cur_y_) ? 1 : get_last_char_idx(cur_y_));
                    text_buffer_.pop_back();

                    auto size = text_buffer_.size();

                    if (cur_y_ == (size_.height - 2) && size >= (size_.height - 2))
                    {
                        auto it = text_buffer_.rbegin();
                        size -= (size_.height - 2);

                        for (uint64_t y = (size_.height - 2); y > 0; --y)
                        {
                            for (uint64_t x = 0; x < (*it).length(); ++x)
                            {
                                buffer_[y * size_.width + x + 1] = (*it)[x];
                            }
                            for (uint64_t x = (*it).length() + 1; x < size_.width - 2; ++x)
                            {
                                buffer_[y * size_.width + x] = U_SPACE;
                            }

                            ++it;
                        }
                    }
                    else
                    {
                        --cur_y_;
                    }

                    word_len = get_last_word_len(cur_y_);

                    cur_x_ = (is_line_empty(cur_y_) ? 1 : get_last_char_idx(cur_y_));
                    it_ = text_buffer_.end() - 1;

                    return;
                }

                --cur_x_;
                --word_len;

                buffer_[(cur_y_ * size_.width) + cur_x_] = U_SPACE;
                if (!(*it_).empty())
                    (*it_).pop_back();

                return;
                break;
            case ' ':
                word_len = 0;
                c = U_SPACE;
                break;
            case '\n':
                ++cur_y_;
                cur_x_ = 1;
                word_len = 0;

                text_buffer_.push_back("");
                it_ = text_buffer_.end() - 1;

                if (cur_y_ >= size_.height - 1)
                    scroll_up();
                return;
                break;
            case '\r':
                cur_x_ = 1;
                return;
                break;
            }

            if (cur_x_ == size_.width - 1)
            {
                uint64_t cursor_tmp = 1;

                text_buffer_.push_back("");
                it_ = text_buffer_.end() - 1;

                if ((cur_y_ + 1) >= size_.height - 1)
                    scroll_up(false);
                else
                    ++cur_y_;

                if (word_len > 0 && word_len < (size_.width - 2))
                {
                    uint64_t word_len_copy = (word_len - 1);

                    for (; word_len_copy > 0; --word_len_copy)
                    {
                        buffer_[cur_y_ * size_.width + cursor_tmp] =
                            buffer_[((cur_y_ - 1) * size_.width) + (size_.width - word_len_copy) - 1];
                        buffer_[((cur_y_ - 1) * size_.width) + (size_.width - word_len_copy) - 1] = U_SPACE;

                        char tmp = (it_ - 1)->back();
                        it_->push_back(tmp);
                        (it_ - 1)->pop_back();

                        ++cursor_tmp;
                    }

                    cur_x_ = cursor_tmp;
                }
                else
                {
                    cur_x_ = 1;
                    word_len = 1;
                }
            }
            else if (cur_y_ >= size_.height - 1)
                scroll_up();

            buffer_[(cur_y_ * size_.width) + cur_x_] = c;
            it_->push_back(c);

            ++word_len;
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

        uint64_t get_last_word_len(uint64_t line)
        {
            uint64_t len = 1;

            uint64_t last_char_idx = get_last_char_idx(line);

            for (uint64_t x = last_char_idx - 1; x > 0; --x)
            {
                if (buffer_[line * size_.width + x] == U_SPACE)
                    break;

                ++len;
            }

            return len;
        }

        std::vector<std::string> text_buffer_;
        std::vector<std::string>::iterator it_;
    };
}

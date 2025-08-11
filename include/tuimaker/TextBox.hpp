#pragma once

#include <deque>
#include <tuimaker/Window.hpp>

namespace tmk
{
    class TextBox : public Window
    {
    public:
        TextBox(Size wsize)
            : Window(wsize)
        {
            text_buffer_.push_back(L"");
            current_line_it_ = text_buffer_.end() - 1;
        };

        ~TextBox() {
        };

        void write(const std::wstring &str)
        {
            for (int i = 0; i < str.length(); ++i)
                write_char(str[i]);
        }

        void write_char(char c)
        {
            static uint64_t word_len = 0;
            const uint64_t cur_default_x = (conf_.border_visible ? 1 : 0);
            const uint64_t cur_default_y = (conf_.border_visible ? 1 : 0);
            [[maybe_unused]] auto [width_lim, height_lim] = size_.get_loop_end(conf_.border_visible);

            switch (c)
            {
            // Backspace
            case 127:
                if (cursor_.x == cur_default_x && cursor_.y == cur_default_y)
                {
                    return;
                }
                else if (cursor_.x == cur_default_x && cursor_.y > cur_default_y)
                {
                    scroll_up();
                    return;
                }

                --cursor_.x;
                --word_len;

                buffer_[(cursor_.y * size_.width) + cursor_.x].character = TChar::U_SPACE;
                if (!(*current_line_it_).empty())
                    (*current_line_it_).pop_back();

                return;
                break;
            case ' ':
                word_len = 0;
                c = TChar::U_SPACE;
                break;
            case '\n':
                word_len = 0;

                if (cursor_.y + 1 >= height_lim)
                {
                    scroll_down();
                }
                else
                {
                    text_buffer_.push_back(L"");
                    current_line_it_ = text_buffer_.end() - 1;
                    ++last_show_line_idx_;
                    ++cursor_.y;
                }

                cursor_.reset_x(conf_.border_visible);

                return;
                break;
            case '\r':
                cursor_.reset_x(conf_.border_visible);
                return;
                break;
            }

            if (cursor_.x == size_.width - 1)
            {
                uint64_t cursor_tmp = cur_default_x;

                text_buffer_.push_back(L"");
                current_line_it_ = text_buffer_.end() - 1;

                if ((cursor_.y + 1) >= size_.height - cur_default_y)
                    scroll_down(false);

                if (word_len > 0 && word_len < (size_.width - cur_default_x - 1))
                {
                    wrap_current_line();
                }
                else
                {
                    cursor_.reset_x(conf_.border_visible);
                    word_len = 1;
                }
            }
            else if (cursor_.y >= height_lim)
                scroll_down();

            buffer_[(cursor_.y * size_.width) + cursor_.x].character = c;
            *current_line_it_ += c;

            ++word_len;
            ++cursor_.x;
        }

        void scroll_up(bool reset_cursor_x = true)
        {
            [[maybe_unused]] auto [w, h] = size_.get_loop_start(conf_.border_visible);
            [[maybe_unused]] auto [width_lim, height_lim] = size_.get_loop_end(conf_.border_visible);
            std::deque<std::wstring>::reverse_iterator it(current_line_it_ - (height_lim - 1));

            if (++it == text_buffer_.rend())
            {
                text_buffer_.push_front(L"");
                ++last_show_line_idx_;
            }

            --current_line_it_;
            --last_show_line_idx_;
            cursor_.y = h;

            if (reset_cursor_x)
                cursor_.reset_x(conf_.border_visible);
            else
                cursor_.y = get_last_word_len(std::distance(text_buffer_.begin(), current_line_it_));

            refresh_buffer();
        }

        void scroll_down(bool reset_cursor_x = true)
        {
            [[maybe_unused]] auto [width_lim, height_lim] = size_.get_loop_end(conf_.border_visible);

            if (++current_line_it_ == text_buffer_.end())
            {
                text_buffer_.push_back(L"");
                current_line_it_ = text_buffer_.end() - 1;
            }

            ++last_show_line_idx_;
            cursor_.y = height_lim - 1;

            if (reset_cursor_x)
                cursor_.reset_x(conf_.border_visible);
            else
                cursor_.y = get_last_word_len(std::distance(text_buffer_.begin(), current_line_it_));

            refresh_buffer();
        }

        void wrap_current_line(void)
        {
            uint64_t last_word_len = get_last_word_len(std::distance(text_buffer_.begin(), current_line_it_));
            std::wstring str = L"";
            std::wstring current_str = *current_line_it_;
            auto it = current_str.rbegin();

            for (uint64_t i = 0; i < last_word_len; ++i)
            {
                str += *it;
                current_str.pop_back();
                ++it;
            }

            text_buffer_.insert(current_line_it_ + 1, str);
            cursor_.reset_x(conf_.border_visible);

            cursor_.x = last_word_len;
            ++current_line_it_;
            ++cursor_.y;

            refresh_buffer();
        }

    private:
        void refresh_buffer(void)
        {
            [[maybe_unused]] auto [w, h] = size_.get_loop_start(conf_.border_visible);
            [[maybe_unused]] auto [width_lim, height_lim] = size_.get_loop_end(conf_.border_visible);
            uint64_t count = (last_show_line_idx_ > (height_lim - 1)
                                  ? (last_show_line_idx_ - (last_show_line_idx_ - (height_lim - 1)))
                                  : last_show_line_idx_);
            auto it = text_buffer_.begin() + last_show_line_idx_ - 1;

            for (uint64_t i = 0; i < count; ++i)
            {
                uint64_t x = w;
                --height_lim;

                for (char c : *it)
                {
                    buffer_[height_lim * size_.width + x].character = c;
                    ++x;
                }
                // Clear the rest of the line
                for (; x < width_lim; ++x)
                    buffer_[height_lim * size_.width + x].character = TChar::U_SPACE;

                --it;
            }
        }

        uint64_t get_last_word_len(uint64_t line)
        {
            auto str = text_buffer_.at(line);
            size_t idx = str.find_last_of(L" ");

            return (idx != std::wstring::npos ? str.length() - idx - 1 : str.length());
        }

        uint64_t last_show_line_idx_ = 0;
        std::deque<std::wstring> text_buffer_;
        std::deque<std::wstring>::iterator current_line_it_;
    };
}

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
            [[maybe_unused]] auto [cur_def_x, cur_def_y] = size_.get_loop_start(conf_.border_visible);
            [[maybe_unused]] auto [width_lim, height_lim] = size_.get_loop_end(conf_.border_visible);

            switch (c)
            {
            // Backspace
            case 127:
            {
                if (current_line_it_->empty())
                {
                    delete_line();

                    uint64_t curr_line_pos_buff = std::distance(text_buffer_.begin(), current_line_it_);

                    scroll_up(false);

                    refresh_buffer(cur_def_y);
                    cursor_.x = current_line_it_->length() + cur_def_x;
                }
                else
                {

                    if (cursor_.x > cur_def_x)
                        --cursor_.x;

                    (*current_line_it_).pop_back();

                    buffer_[(cursor_.y * size_.width) + cursor_.x].character = TChar::U_SPACE;
                }

                return;
            }
            break;
            case '\n':
            {
                if (cursor_.y < (height_lim - cur_def_y))
                    ++cursor_.y;
                else
                    ++first_show_line_idx_;

                cursor_.reset_x(conf_.border_visible);

                add_line();
                refresh_buffer(cur_def_y);

                return;
            }
            break;
            case '\r':
            {
                //! Implement

                return;
            }
            break;
            }

            *current_line_it_ += c;

            if (cursor_.x < width_lim)
            {
                buffer_[(cursor_.y * size_.width) + cursor_.x].character = c;
                ++cursor_.x;
            }
            else
            {
                if (cursor_.y == (height_lim - 1))
                {
                    ++first_show_line_idx_;
                }

                refresh_buffer(first_show_line_idx_);
            }
        }

        void add_line(void)
        {
            current_line_it_ = text_buffer_.insert(current_line_it_ + 1, L"");
        }

        void delete_line(void)
        {
            if (current_line_it_ != text_buffer_.begin())
                current_line_it_ = text_buffer_.erase(current_line_it_);
        }

        void scroll_up(const bool &refresh = true)
        {
            [[maybe_unused]] auto [w, h] = size_.get_loop_start(conf_.border_visible);
            [[maybe_unused]] auto [width_lim, height_lim] = size_.get_loop_end(conf_.border_visible);
            std::deque<std::wstring>::reverse_iterator it(current_line_it_);
            if (first_show_line_idx_ > 0)
                --first_show_line_idx_;
            else if (cursor_.y > h) [[likely]]
                --cursor_.y;

            if (current_line_it_ > text_buffer_.begin())
                --current_line_it_;

            if (refresh)
                refresh_buffer(h);
        }

        void
        scroll_down(const bool &alterate_buffer = true)
        {
            [[maybe_unused]] auto [w, h] = size_.get_loop_start(conf_.border_visible);
            [[maybe_unused]] auto [width_lim, height_lim] = size_.get_loop_end(conf_.border_visible);

            if (cursor_.y < (height_lim - h))
                ++cursor_.y;

            if (alterate_buffer && ++current_line_it_ == text_buffer_.end())
            {
                text_buffer_.push_back(L"");
                current_line_it_ = text_buffer_.end() - 1;
            }

            if (text_buffer_.size() > (height_lim - h))
                ++first_show_line_idx_;
        }

    private:
        void refresh_buffer(const uint64_t &buff_line_offset = 0)
        {
            [[maybe_unused]] auto [w, h] = size_.get_loop_start(conf_.border_visible);
            [[maybe_unused]] auto [width_lim, height_lim] = size_.get_loop_end(conf_.border_visible);
            auto it = text_buffer_.begin() + first_show_line_idx_;
            uint64_t offset = h;
            uint64_t x = w;

            for (; it != text_buffer_.end() && offset < height_lim; ++it)
            {
                uint64_t line_char_count = 0;
                uint64_t word_len = 0;

                x = w;

                for (char c : *it)
                {
                    if (c == ' ')
                        word_len = 1;

                    if (x >= width_lim)
                    {
                        for (uint64_t i = 0; i < word_len; ++i)
                        {
                            wchar_t c = buffer_[offset * size_.width + width_lim - i - w].character;
                            buffer_[offset * size_.width + width_lim - i - w].character = TChar::U_SPACE;
                            buffer_[(offset + 1) * size_.width + word_len - i].character = c;
                        }

                        line_char_count = 0;
                        x = word_len;
                        ++offset;
                    }

                    buffer_[offset * size_.width + x].character = c;
                    ++word_len;
                    ++line_char_count;
                    ++x;
                }

                for (; line_char_count < (width_lim - h); ++line_char_count)
                    buffer_[offset * size_.width + line_char_count + w].character = TChar::U_SPACE;

                ++offset;
            }
        }

        uint64_t first_show_line_idx_ = 0;
        std::deque<std::wstring> text_buffer_;
        std::deque<std::wstring>::iterator current_line_it_;
    };
}

#pragma once

#include <deque>
#include <cmath>
#include <tuimaker/Window.hpp>

namespace tmk
{
    class TextBox : public Window
    {
    public:
        TextBox(Size wsize, const Conf &conf = {true, TChar::TC_DEFAULT, TChar::BGC_DEFAULT})
            : Window(wsize, conf)
        {
            text_buffer_.push_front(L"");
            current_line_it_ = text_buffer_.begin();
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
                    scroll_up(false);
                }
                else
                {
                    std::lock_guard<std::mutex> lock(mtx_);

                    uint64_t old_len = current_line_it_->length();
                    current_line_it_->pop_back();
                }

                refresh_buffer();
                return;
            }
            break;
            case '\n':
            {
                { // Mutex lock
                    std::lock_guard<std::mutex> lock(mtx_);
                    if (cursor_.y < (height_lim - cur_def_y))
                        ++cursor_.y;
                    else
                        ++first_show_line_idx_;

                    cursor_.reset_x(conf_.border_visible);
                } // Mutex lock end

                add_line();
                refresh_buffer();

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

            std::lock_guard<std::mutex> lock(mtx_);
            *current_line_it_ += c;

            if (cursor_.x < width_lim)
            {
                buffer_[(cursor_.y * size_.width) + cursor_.x].character = c;
                ++cursor_.x;
            }
            else
            {
                uint64_t len = current_line_it_->length();

                if (cursor_.y == (height_lim - 1))
                    ++first_show_line_idx_;

                refresh_buffer();
            }
        }

        void add_line(void)
        {
            std::lock_guard<std::mutex> lock(mtx_);

            current_line_it_ = text_buffer_.insert(current_line_it_ + 1, L"");
        }

        void delete_line(void)
        {
            std::lock_guard<std::mutex> lock(mtx_);

            if (current_line_it_ != text_buffer_.begin())
                current_line_it_ = text_buffer_.erase(current_line_it_);
        }

        void scroll_up(const bool &refresh = true)
        {
            std::lock_guard<std::mutex> lock(mtx_);

            [[maybe_unused]] auto [w, h] = size_.get_loop_start(conf_.border_visible);
            [[maybe_unused]] auto [width_lim, height_lim] = size_.get_loop_end(conf_.border_visible);
            std::deque<std::wstring>::reverse_iterator it(current_line_it_);

            if (first_show_line_idx_ > 0)
                --first_show_line_idx_;

            if (current_line_it_ > text_buffer_.begin())
                --current_line_it_;

            if (refresh)
                refresh_buffer();
        }

        void scroll_down(const bool &refresh = true)
        {
            std::lock_guard<std::mutex> lock(mtx_);

            [[maybe_unused]] auto [w, h] = size_.get_loop_start(conf_.border_visible);
            [[maybe_unused]] auto [width_lim, height_lim] = size_.get_loop_end(conf_.border_visible);

            if (text_buffer_.size() > (height_lim - h))
                ++first_show_line_idx_;

            if (current_line_it_ < (text_buffer_.end() - 1))
                ++current_line_it_;

            if (refresh)
                refresh_buffer();
        }

    private:
        uint64_t get_line_wrap_idx(const std::deque<std::wstring>::iterator &it,
                                   uint64_t first_char_idx, uint64_t last_char_idx)
        {
            if (last_char_idx > it->length())
                last_char_idx = it->length();

            for (uint64_t i = (last_char_idx - 1); i >= first_char_idx && i != UINT64_MAX; --i)
                if (it->at(i) == ' ')
                    return i;

            return last_char_idx;
        }

        void refresh_buffer(void)
        {
            static uint64_t old_line_count = 0;
            [[maybe_unused]] auto [w, h] = size_.get_loop_start(conf_.border_visible);
            [[maybe_unused]] auto [width_lim, height_lim] = size_.get_loop_end(conf_.border_visible);
            auto it = text_buffer_.begin() + first_show_line_idx_;
            uint64_t offset = h;

            for (; it != text_buffer_.end() && offset < height_lim; ++it)
            {
                uint64_t line_char_count = 0;
                uint64_t len = it->length();
                uint64_t x = w;

                // Check if current line need wrapping
                if (len > (width_lim - w))
                {
                    for (uint64_t i = 0; i < len && offset < height_lim;)
                    {
                        uint64_t limit = (i + (width_lim - w));
                        uint64_t res = ((len - i) < (width_lim - w)
                                            ? len
                                            : get_line_wrap_idx(it, i, limit));

                        line_char_count = 0;
                        x = w;

                        for (; i < res; ++i)
                        {
                            buffer_[offset * size_.width + x].character = it->at(i);
                            ++line_char_count;
                            ++x;
                        }

                        if (res < limit && res != len)
                            ++i;

                        for (; line_char_count < (width_lim - w); ++line_char_count)
                            buffer_[offset * size_.width + line_char_count + w].character = TChar::U_SPACE;

                        ++offset;
                    }
                }
                else
                {
                    for (char c : *it)
                    {
                        buffer_[offset * size_.width + x].character = c;
                        ++line_char_count;
                        ++x;
                    }

                    for (; line_char_count < (width_lim - h); ++line_char_count)
                        buffer_[offset * size_.width + line_char_count + w].character = TChar::U_SPACE;

                    ++offset;
                }

                // Set cursor position
                if (it == current_line_it_)
                {
                    cursor_.x = x;
                    cursor_.y = (offset - 1);
                }
            }

            if (offset < old_line_count)
                for (uint64_t i = offset; i < old_line_count; ++i)
                    for (uint64_t x = w; x < width_lim; ++x)
                        buffer_[i * size_.width + x].character = TChar::U_SPACE;

            old_line_count = offset;
        }

        uint64_t first_show_line_idx_ = 0;
        std::deque<std::wstring> text_buffer_;
        std::deque<std::wstring>::iterator current_line_it_;
    };
}

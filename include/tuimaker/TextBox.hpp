#pragma once

#include <deque>
#include <cmath>
#include <tuimaker/Window.hpp>

namespace tmk
{
    /**
     * @class TextBox
     * @brief Text window
     *
     */
    class TextBox : public Window
    {
    public:
        /**
         * @brief Construct a new Text Box object
         *
         * @param title Window title
         * @param wsize Window size
         * @param conf Window configuration
         */
        TextBox(const std::string &title, Size wsize, const Conf &conf)
            : Window(title, wsize, conf)
        {
            text_buffer_.push_front(L"");
            current_line_it_ = text_buffer_.begin();
        };

        /**
         * @brief Destroy the Text Box object
         *
         */
        ~TextBox() {
        };

        /**
         * @brief Write a string
         *
         * @param str
         */
        auto write(const std::wstring &str) -> void
        {
            for (int i = 0; i < str.length(); ++i)
                write_char(str[i]);
        }

        /**
         * @brief Write a single char
         * It has line wrap and left align
         * @param c
         */
        auto write_char(char c) -> void
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

        /**
         * @brief Add a new line in the text buffer after the current line
         *
         */
        auto add_line(void) -> void
        {
            std::lock_guard<std::mutex> lock(mtx_);

            current_line_it_ = text_buffer_.insert(current_line_it_ + 1, L"");
        }

        /**
         * @brief Delete the current line
         *
         */
        auto delete_line(void) -> void
        {
            std::lock_guard<std::mutex> lock(mtx_);

            if (current_line_it_ != text_buffer_.begin())
                current_line_it_ = text_buffer_.erase(current_line_it_);
        }

        /**
         * @brief Scroll up by one line
         *
         * @param refresh
         */
        auto scroll_up(const bool &refresh = true) -> void
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

        /**
         * @brief Scroll down by one line
         *
         * @param refresh
         */
        auto scroll_down(const bool &refresh = true) -> void
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
        /**
         * @brief Calculate the next point of wrap of the line
         *
         * @param it Buffer iterator to the line to wrap
         * @param first_char_idx Index of the first char of the line
         * @param last_char_idx Index of the last char in the line
         * @return uint64_t
         */
        auto get_line_wrap_idx(const std::deque<std::wstring>::iterator &it,
                               uint64_t first_char_idx, uint64_t last_char_idx) -> uint64_t
        {
            if (last_char_idx > it->length())
                last_char_idx = it->length();

            for (uint64_t i = (last_char_idx - 1); i >= first_char_idx && i != UINT64_MAX; --i)
                if (it->at(i) == ' ')
                    return i;

            return last_char_idx;
        }

        /**
         * @brief Update the window buffer with the text buffer
         *
         */
        auto refresh_buffer(void) -> void
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

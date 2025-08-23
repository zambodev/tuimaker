#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <memory>
#include <mutex>
#include <tuimaker/TChar.hpp>
#include <tuimaker/TermUtils.hpp>

namespace tmk
{
    /**
     * @class Window
     * @brief Base window class
     *
     */
    class Window
    {
    public:
        /**
         * @brief Window id
         *
         */
        typedef uint64_t Id;

        /**
         * @brief Window cursor
         * Used for text input
         */
        struct Cursor
        {
            auto reset_x(bool border_visible) -> void
            {
                x = (border_visible ? 1 : 0);
            }

            auto reset_y(bool border_visible) -> void
            {
                y = (border_visible ? 1 : 0);
            }

            uint64_t x = 0;
            uint64_t y = 0;
        };

        /**
         * @brief Window size
         *
         */
        struct Size
        {
            uint64_t x = 0;
            uint64_t y = 0;
            uint64_t width = 0;
            uint64_t height = 0;

            auto get_loop_start(bool border_visible) -> std::pair<uint64_t, uint64_t>
            {
                if (border_visible)
                    return {1, 1};
                else
                    return {0, 0};
            }

            auto get_loop_end(bool border_visible) -> std::pair<uint64_t, uint64_t>
            {
                if (border_visible)
                    return {width - 1, height - 1};
                else
                    return {width, height};
            }
        };

        /**
         * @brief Window configuration
         * Set borders visibility and colors
         */
        struct Conf
        {
            bool border_visible;
            const wchar_t *border_color;
            const wchar_t *text_color;
            const wchar_t *bg_color;
        };

        Window() = delete;

        /**
         * @brief Construct a new Window object
         *
         * @param title Window title
         * @param wsize Window size
         * @param conf Window conf
         */
        Window(const std::string &title, const Size &wsize, const Conf &conf)
            : buffer_(std::make_shared<TChar[]>(size_.width * size_.height)),
              size_(wsize),
              conf_(conf),
              id_(TermUtils::get_progressive_id()),
              title_(title)
        {
            // Set cursor position
            cursor_.reset_x(conf_.border_visible);
            cursor_.reset_y(conf_.border_visible);
            // Color text and background
            for (uint64_t h = 0; h < size_.height; ++h)
            {
                for (uint64_t w = 0; w < size_.width; ++w)
                {
                    buffer_[h * size_.width + w].text_color = conf_.text_color;
                    buffer_[h * size_.width + w].bg_color = conf_.bg_color;
                }
            }
            // Draw borders
            if (conf_.border_visible)
                draw_borders(conf_.border_color);
        }

        /**
         * @brief Destroy the Window object
         *
         */
        virtual ~Window()
        {
        }

        /**
         * @brief Compare window
         * Comparison made by id check
         * @param window Window object
         * @return true
         * @return false
         */
        auto operator==(Window &window) -> bool
        {
            std::lock_guard<std::mutex> lock(mtx_);

            return (id_ == window.get_id());
        }

        /**
         * @brief Get the Window size
         *
         * @return const Size&
         */
        auto get_size(void) const -> const Size &
        {
            std::lock_guard<std::mutex> lock(mtx_);

            return size_;
        }

        /**
         * @brief Get the window id
         *
         * @return Id
         */
        auto get_id(void) const -> Id
        {
            return id_;
        }

        /**
         * @brief Get the window screen buffer
         *
         * @return std::shared_ptr<const TChar[]>
         */
        auto get_buffer(void) -> std::shared_ptr<const TChar[]>
        {
            std::lock_guard<std::mutex> lock(mtx_);

            return buffer_;
        }

        /**
         * @brief Get (x, y) char of window screen buffer
         *
         * @param x
         * @param y
         * @return TChar&
         */
        auto get_char_at(uint64_t x, int64_t y) -> TChar &
        {
            return buffer_[y * size_.width + x];
        }

        /**
         * @brief Show window cursor at current position
         *
         */
        auto show_cursor(void) -> void
        {
            std::lock_guard<std::mutex> lock(mtx_);

            std::wcout << std::format(L"\e[{};{}H\e[?25h", size_.y + cursor_.y + 1, size_.x + cursor_.x + 1);
        }

        /**
         * @brief Set the background color
         *
         * @param color
         */
        auto set_bg_color(const wchar_t *color) -> void
        {
            std::lock_guard<std::mutex> lock(mtx_);

            for (uint64_t h = 0; h < size_.height; ++h)
                for (uint64_t w = 0; w < size_.width; ++w)
                    buffer_[h * size_.width + w].bg_color = color;
        }

        /**
         * @brief Set the text color
         *
         * @param color
         */
        auto set_text_color(const wchar_t *color) -> void
        {
            std::lock_guard<std::mutex> lock(mtx_);

            auto [w, h] = size_.get_loop_start(conf_.border_visible);
            auto [width_lim, height_lim] = size_.get_loop_end(conf_.border_visible);

            for (; h < height_lim; ++h)
                for (; w < width_lim; ++w)
                    buffer_[h * size_.width + w].bg_color = color;
        }

        /**
         * @brief Set the border color
         *
         * @param color
         */
        auto set_border_color(const wchar_t *color) -> void
        {
            std::lock_guard<std::mutex> lock(mtx_);

            draw_borders(color);
        }

        /**
         * @brief Set the cursor position
         *
         * @param x
         * @param y
         */
        auto set_cursor_pos(int x, int y) -> void
        {
            std::lock_guard<std::mutex> lock(mtx_);

            cursor_.x = x;
            cursor_.y = y;
        }

        /**
         * @brief Set the window selected
         *
         * @param state
         */
        auto select(bool state) -> void
        {
            std::lock_guard<std::mutex> lock(mtx_);
            is_selected_ = state;
        }

        /**
         * @brief Check if window border is visible
         *
         * @return true
         * @return false
         */
        auto is_border_visible(void) -> bool
        {
            return conf_.border_visible;
        }

    protected:
        mutable std::mutex mtx_;
        bool is_selected_ = false;
        std::string title_ = "";
        Cursor cursor_;
        Size size_;
        Conf conf_;
        Id id_;
        std::shared_ptr<TChar[]> buffer_ = nullptr;

    private:
        /**
         * @brief Draw window borders
         *
         * @param color
         */
        auto draw_borders(const wchar_t *color) -> void
        {
            // Top and bottom sides
            for (uint64_t i = 1; i < size_.width - 1; ++i)
            {
                buffer_[i].character = TChar::U_BAR_HORIZONTAL;
                buffer_[i].text_color = color;
                buffer_[size_.width * (size_.height - 1) + i].character = TChar::U_BAR_HORIZONTAL;
                buffer_[size_.width * (size_.height - 1) + i].text_color = color;
            }
            // Left and right sides
            for (uint64_t i = 1; i < size_.height - 1; ++i)
            {
                buffer_[i * size_.width].character = TChar::U_BAR_VERTICAL;
                buffer_[i * size_.width].text_color = color;
                buffer_[i * size_.width + (size_.width - 1)].character = TChar::U_BAR_VERTICAL;
                buffer_[i * size_.width + (size_.width - 1)].text_color = color;
            }
            // Corners
            buffer_[0].character = TChar::U_CRN_TOP_LEFT;
            buffer_[0].text_color = color;
            buffer_[size_.width - 1].character = TChar::U_CRN_TOP_RIGHT;
            buffer_[size_.width - 1].text_color = color;
            buffer_[(size_.height - 1) * size_.width].character = TChar::U_CRN_BOTTOM_LEFT;
            buffer_[(size_.height - 1) * size_.width].text_color = color;
            buffer_[(size_.height - 1) * size_.width + (size_.width - 1)].character = TChar::U_CRN_BOTTOM_RIGHT;
            buffer_[(size_.height - 1) * size_.width + (size_.width - 1)].text_color = color;
            // Draw title
            if (title_ != "")
            {
                auto [w, h] = size_.get_loop_start(conf_.border_visible);
                auto [width_lim, height_lim] = size_.get_loop_end(conf_.border_visible);
                uint64_t x = (((width_lim - w) - title_.length()) / 2) + 1;

                for (char c : title_)
                {
                    buffer_[x].character = c;
                    ++x;
                }
            }
        }
    };
}

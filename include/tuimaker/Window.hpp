#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <memory>
#include <tuimaker/TChar.hpp>
#include <tuimaker/TermUtils.hpp>

namespace tmk
{
    // Incomplete declaration to have the type definen for the variant
    // and for the variant to be defined during Window declaration
    class Window;
    class TextBox;

    class Window
    {
    public:
        typedef uint64_t Id;

        struct Cursor
        {
            void reset_x(bool border_visible)
            {
                x = (border_visible ? 1 : 0);
            }

            void reset_y(bool border_visible)
            {
                y = (border_visible ? 1 : 0);
            }

            uint64_t x = 0;
            uint64_t y = 0;
        };

        struct Size
        {
            uint64_t x = 0;
            uint64_t y = 0;
            uint64_t width = 0;
            uint64_t height = 0;

            std::pair<uint64_t, uint64_t> get_loop_start(bool border_visible)
            {
                if (border_visible)
                    return {1, 1};
                else
                    return {0, 0};
            }

            std::pair<uint64_t, uint64_t> get_loop_end(bool border_visible)
            {
                if (border_visible)
                    return {width - 1, height - 1};
                else
                    return {width, height};
            }
        };

        struct Conf
        {
            bool border_visible;
            const wchar_t *text_color;
            const wchar_t *bg_color;
        };

        Window() = delete;
        Window(const Size &wsize, const Conf &conf = {true, TChar::TC_DEFAULT, TChar::BGC_DEFAULT})
            : buffer_(std::make_shared<TChar[]>(size_.width * size_.height)),
              size_(wsize),
              conf_(conf),
              id_(TermUtils::get_progressive_id()),
              cursor_(conf_.border_visible)
        {
            cursor_.reset_x(conf_.border_visible);
            cursor_.reset_y(conf_.border_visible);

            if (conf_.border_visible)
                draw_borders(conf_.text_color);
        }

        virtual ~Window()
        {
        }

        bool operator==(Window &window)
        {
            if (id_ == window.get_id())
                return true;
            else
                return false;
        }

        const Size &get_size(void) const
        {
            return size_;
        }

        Id get_id(void) const
        {
            return id_;
        }

        std::shared_ptr<const TChar[]> get_buffer(void)
        {
            return buffer_;
        }

        TChar &get_char_at(uint64_t x, int64_t y)
        {
            return buffer_[y * size_.width + x];
        }

        void show_cursor(void)
        {
            std::wcout << std::format(L"\e[{};{}H\e[?25h", size_.y + cursor_.y + 1, size_.x + cursor_.x + 1);
        }

        void set_bg_color(const wchar_t *color)
        {
            for (uint64_t h = 0; h < size_.height; ++h)
                for (uint64_t w = 0; w < size_.width; ++w)
                    buffer_[h * size_.width + w].bg_color = color;
        }

        void set_text_color(const wchar_t *color)
        {
            auto [w, h] = size_.get_loop_start(conf_.border_visible);
            auto [width_lim, height_lim] = size_.get_loop_end(conf_.border_visible);

            for (; h < height_lim; ++h)
                for (; w < width_lim; ++w)
                    buffer_[h * size_.width + w].bg_color = color;
        }

        void set_border_color(const wchar_t *color)
        {
            draw_borders(color);
        }

        void set_cursor_pos(int x, int y)
        {
            cursor_.x = x;
            cursor_.y = y;
        }

        void select(bool state)
        {
            is_selected_ = state;
        }

        bool is_border_visible(void)
        {
            return conf_.border_visible;
        }

    protected:
        bool is_selected_ = false;
        Cursor cursor_;
        Size size_;
        Conf conf_;
        Id id_;
        std::shared_ptr<TChar[]> buffer_ = nullptr;

    private:
        void draw_borders(const wchar_t *color)
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
        }
    };
}

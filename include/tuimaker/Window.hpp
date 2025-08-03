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

    typedef uint64_t WindowId;

    struct WindowSize
    {
        uint64_t x;
        uint64_t y;
        uint64_t width;
        uint64_t height;
    };

    class Window
    {
    public:
        Window() = delete;
        Window(WindowSize wsize)
            : buffer_(std::make_shared<TChar[]>(size_.width * size_.height)),
              size_(wsize),
              id_(TermUtils::get_progressive_id()),
              cur_x_(1), cur_y_(1)
        {
            // Top and bottom sides
            for (uint64_t i = 1; i < size_.width - 1; ++i)
            {
                buffer_[i].character = TChar::U_BAR_HORIZONTAL;
                buffer_[size_.width * (size_.height - 1) + i].character = TChar::U_BAR_HORIZONTAL;
            }
            // Left and right sides
            for (uint64_t i = 1; i < size_.height - 1; ++i)
            {
                buffer_[i * size_.width].character = TChar::U_BAR_VERTICAL;
                buffer_[i * size_.width + (size_.width - 1)].character = TChar::U_BAR_VERTICAL;
            }
            // Corners
            buffer_[0].character = TChar::U_CRN_TOP_LEFT;
            buffer_[size_.width - 1].character = TChar::U_CRN_TOP_RIGHT;
            buffer_[(size_.height - 1) * size_.width].character = TChar::U_CRN_BOTTOM_LEFT;
            buffer_[(size_.height - 1) * size_.width + (size_.width - 1)].character = TChar::U_CRN_BOTTOM_RIGHT;

            // Clear the rest
            for (uint64_t i = 1; i < size_.height - 1; ++i)
                for (uint64_t j = 1; j < size_.width - 1; ++j)
                    buffer_[i * size_.width + j].character = TChar::U_SPACE;
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

        const WindowSize &get_size(void) const
        {
            return size_;
        }

        WindowId get_id(void) const
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

        void set_cursor_pos(int x, int y)
        {
            cur_x_ = x;
            cur_y_ = y;
        }

        void select(bool state)
        {
            is_selected_ = state;
        }

    protected:
        bool is_selected_ = false;
        uint64_t cur_x_;
        uint64_t cur_y_;
        WindowSize size_;
        WindowId id_;
        std::shared_ptr<TChar[]> buffer_ = nullptr;
    };
}

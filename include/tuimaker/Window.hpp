#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <memory>
#include <tuimaker/Utils.hpp>

namespace tmk
{
    // Incomplete declaration to have the type definen for the variant
    // and for the variant to be defined during Window declaration
    class Window;
    class TextBox;

    typedef uint64_t WindowId;

    enum : wchar_t
    {
        U_BAR_HORIZONTAL = u'\u2501',
        U_BAR_VERTICAL = u'\u2503',
        U_CRN_TOP_LEFT = u'\u250f',
        U_CRN_TOP_RIGHT = u'\u2513',
        U_CRN_BOTTOM_LEFT = u'\u2517',
        U_CRN_BOTTOM_RIGHT = u'\u251b',
        U_CROSS = u'\u254b',
        U_T_LEFT = u'\u2523',
        U_T_RIGHT = u'\u252b',
        U_T_TOP = u'\u2533',
        U_T_BOTTOM = u'\u253B',
        U_SPACE = u'\u0020'
    };

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
        Window(WindowSize wsize)
            : buffer_(std::make_shared<wchar_t[]>(size_.width * size_.height)),
              size_(wsize),
              id_(Utils::get_progressive_id()),
              cur_x_(1), cur_y_(1)
        {
            // Top and bottom sides
            for (uint64_t i = 1; i < size_.width - 1; ++i)
            {
                buffer_[i] = U_BAR_HORIZONTAL;
                buffer_[size_.width * (size_.height - 1) + i] = U_BAR_HORIZONTAL;
            }
            // Left and right sides
            for (uint64_t i = 1; i < size_.height - 1; ++i)
            {
                buffer_[i * size_.width] = U_BAR_VERTICAL;
                buffer_[i * size_.width + (size_.width - 1)] = U_BAR_VERTICAL;
            }
            // Corners
            buffer_[0] = U_CRN_TOP_LEFT;
            buffer_[size_.width - 1] = U_CRN_TOP_RIGHT;
            buffer_[(size_.height - 1) * size_.width] = U_CRN_BOTTOM_LEFT;
            buffer_[(size_.height - 1) * size_.width + (size_.width - 1)] = U_CRN_BOTTOM_RIGHT;

            // Clear the rest
            for (uint64_t i = 1; i < size_.height - 1; ++i)
                for (uint64_t j = 1; j < size_.width - 1; ++j)
                    buffer_[i * size_.width + j] = U_SPACE;
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

        std::shared_ptr<const wchar_t[]> get_buffer(void)
        {
            return buffer_;
        }

        void set_cursor_pos(int x, int y)
        {
            cur_x_ = x;
            cur_y_ = y;
        }

        void add_child(WindowId id)
        {
            children_.push_back(id);
        }

        std::vector<WindowId> get_children(void)
        {
            std::vector<WindowId> children(children_);

            return children;
        }

    protected:
        int cur_x_;
        int cur_y_;
        WindowSize size_;
        WindowId id_;
        std::vector<WindowId> children_;
        std::shared_ptr<wchar_t[]> buffer_ = nullptr;
    };
}

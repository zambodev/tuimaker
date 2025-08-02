#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <memory>
#include <tuimaker/TermUtils.hpp>

namespace tmk
{
    // Incomplete declaration to have the type definen for the variant
    // and for the variant to be defined during Window declaration
    class Window;
    class TextBox;

    typedef uint64_t WindowId;

    struct TChar
    {
        // Window borders
        static constexpr wchar_t U_BAR_HORIZONTAL = u'\u2501';
        static constexpr wchar_t U_BAR_VERTICAL = u'\u2503';
        static constexpr wchar_t U_CRN_TOP_LEFT = u'\u250f';
        static constexpr wchar_t U_CRN_TOP_RIGHT = u'\u2513';
        static constexpr wchar_t U_CRN_BOTTOM_LEFT = u'\u2517';
        static constexpr wchar_t U_CRN_BOTTOM_RIGHT = u'\u251b';
        static constexpr wchar_t U_CROSS = u'\u254b';
        static constexpr wchar_t U_T_LEFT = u'\u2523';
        static constexpr wchar_t U_T_RIGHT = u'\u252b';
        static constexpr wchar_t U_T_TOP = u'\u2533';
        static constexpr wchar_t U_T_BOTTOM = u'\u253B';
        static constexpr wchar_t U_SPACE = u'\u0020';
        // Text color
        static constexpr const wchar_t *TC_BLACK = L"\e[30m";
        static constexpr const wchar_t *TC_RED = L"\e[31m";
        static constexpr const wchar_t *TC_GREEN = L"\e[32m";
        static constexpr const wchar_t *TC_YELLOW = L"\e[33m";
        static constexpr const wchar_t *TC_BLUE = L"\e[34m";
        static constexpr const wchar_t *TC_MAGENTA = L"\e[35m";
        static constexpr const wchar_t *TC_CYAN = L"\e[36m";
        static constexpr const wchar_t *TC_WHITE = L"\e[37m";
        // Background color
        static constexpr const wchar_t *BGC_BLACK = L"\e[40m";
        static constexpr const wchar_t *BGC_RED = L"\e[41m";
        static constexpr const wchar_t *BGC_GREEN = L"\e[42m";
        static constexpr const wchar_t *BGC_YELLOW = L"\e[43m";
        static constexpr const wchar_t *BGC_BLUE = L"\e[44m";
        static constexpr const wchar_t *BGC_MAGENTA = L"\e[45m";
        static constexpr const wchar_t *BGC_CYAN = L"\e[46m";
        static constexpr const wchar_t *BGC_WHITE = L"\e[47m";
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
        Window() = delete;
        Window(WindowSize wsize)
            : buffer_(std::make_shared<wchar_t[]>(size_.width * size_.height)),
              size_(wsize),
              id_(TermUtils::get_progressive_id()),
              cur_x_(1), cur_y_(1)
        {
            // Top and bottom sides
            for (uint64_t i = 1; i < size_.width - 1; ++i)
            {
                buffer_[i] = TChar::U_BAR_HORIZONTAL;
                buffer_[size_.width * (size_.height - 1) + i] = TChar::U_BAR_HORIZONTAL;
            }
            // Left and right sides
            for (uint64_t i = 1; i < size_.height - 1; ++i)
            {
                buffer_[i * size_.width] = TChar::U_BAR_VERTICAL;
                buffer_[i * size_.width + (size_.width - 1)] = TChar::U_BAR_VERTICAL;
            }
            // Corners
            buffer_[0] = TChar::U_CRN_TOP_LEFT;
            buffer_[size_.width - 1] = TChar::U_CRN_TOP_RIGHT;
            buffer_[(size_.height - 1) * size_.width] = TChar::U_CRN_BOTTOM_LEFT;
            buffer_[(size_.height - 1) * size_.width + (size_.width - 1)] = TChar::U_CRN_BOTTOM_RIGHT;

            // Clear the rest
            for (uint64_t i = 1; i < size_.height - 1; ++i)
                for (uint64_t j = 1; j < size_.width - 1; ++j)
                    buffer_[i * size_.width + j] = TChar::U_SPACE;
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

        void select(bool state)
        {
            is_selected_ = state;
        }

    protected:
        bool is_selected_ = false;
        uint64_t cur_x_;
        uint64_t cur_y_;
        WindowSize size_;
        const wchar_t *bg_color = TChar::BGC_WHITE;
        const wchar_t *text_color = TChar::BGC_BLACK;
        WindowId id_;
        std::shared_ptr<wchar_t[]> buffer_ = nullptr;
    };
}

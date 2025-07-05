#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <memory>
#include "Utils.h"

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
        int x;
        int y;
        int width;
        int height;
    };

    class Window
    {
    public:
        Window(WindowSize wsize)
            : size_(wsize),
              buffer_(std::make_shared<wchar_t[]>(wsize.width * wsize.height)),
              id_(Utils::get_progressive_id()),
              cur_x_(1), cur_y_(1)
        {
            for (int i = 0; i < wsize.width * wsize.height; ++i)
                buffer_[i] = U_SPACE;

            draw();
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

        std::shared_ptr<wchar_t[]> get_buffer(void)
        {
            return buffer_;
        }

        void draw(void)
        {
            // Top and bottom sides
            for (int i = 1; i < this->size_.width - 1; ++i)
            {
                buffer_[i] = U_BAR_HORIZONTAL;
                buffer_[size_.width * (size_.height - 1) + i] = U_BAR_HORIZONTAL;
            }
            // Left and right sides
            for (int i = 1; i < size_.height - 1; ++i)
            {
                buffer_[i * size_.width] = U_BAR_VERTICAL;
                buffer_[i * size_.width + (size_.width - 1)] = U_BAR_VERTICAL;
            }
            // Corners
            buffer_[0] = U_CRN_TOP_LEFT;
            buffer_[size_.width - 1] = U_CRN_TOP_RIGHT;
            buffer_[(size_.height - 1) * size_.width] = U_CRN_BOTTOM_LEFT;
            buffer_[(size_.height - 1) * size_.width + (size_.width - 1)] = U_CRN_BOTTOM_RIGHT;
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

        virtual void write(const std::string &&str) {}
        virtual void write_char(const char c) {}

    protected:
        int cur_x_;
        int cur_y_;
        std::shared_ptr<wchar_t[]> buffer_;
        WindowSize size_;
        WindowId id_;
        std::vector<WindowId> children_;
    };

    class TextBox : public Window
    {
    public:
        TextBox(WindowSize wsize)
            : Window(wsize)
        {
        };

        ~TextBox()
        {
        };

        void write(const std::string &&str)
        {
            for (int i = 0; i < str.length(); ++i)
                write_char(str[i]);
        }

        void write_char(const char c)
        {
            static int wordBeginIdx = 0;

            if (c == ' ')
                wordBeginIdx = 1;

            if (cur_x_ == size_.width - 1)
            {
                int oldWordBeginIdx = wordBeginIdx;
                int cursorTmp = 1;

                for (; wordBeginIdx < 0; ++wordBeginIdx)
                {
                    buffer_[(cur_y_ + 1) * size_.width + cursorTmp] =
                        buffer_[(cur_y_)*size_.width + cur_x_ + wordBeginIdx];
                    buffer_[(cur_y_)*size_.width + cur_x_ + wordBeginIdx] = ' ';

                    ++cursorTmp;
                }

                cur_x_ = cursorTmp;
                ++cur_y_;
            }

            buffer_[(cur_y_)*size_.width + cur_x_] = c;

            --wordBeginIdx;
            ++cur_x_;
        }

    private:
    };
}

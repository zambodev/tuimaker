#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <memory>
#include <variant>
#include "Utils.h"


namespace tmk
{
    // Incomplete declaration to have the type definen for the variant
    // and for the variant to be defined during Window declaration
    class Window;
    class TextBox;

    typedef uint64_t WindowId;
    typedef std::variant<std::shared_ptr<Window>, std::shared_ptr<TextBox>> WindowType;

    enum : wchar_t
    {
        U_BAR_HORIZONTAL        = u'\u2501',
        U_BAR_VERTICAL          = u'\u2503',
        U_CRN_TOP_LEFT          = u'\u250f',
        U_CRN_TOP_RIGHT         = u'\u2513',
        U_CRN_BOTTOM_LEFT       = u'\u2517',
        U_CRN_BOTTOM_RIGHT      = u'\u251b',
        U_CROSS                 = u'\u254b',
        U_T_LEFT                = u'\u2523',
        U_T_RIGHT               = u'\u252b',
        U_T_TOP                 = u'\u2533',
        U_T_BOTTOM              = u'\u253B',
        U_SPACE                 = u'\u0020'
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
                for(int i = 0; i < wsize.width * wsize.height; ++i)
                    this->buffer_[i] = U_SPACE;

                this->draw();
            }

            ~Window()
            {
            }

            bool operator==(Window &window)
            {
                if(this->id_ == window.get_id())
                    return true;
                else
                    return false;
            }

            const WindowSize &get_size(void) const
            {
                return this->size_;
            }

            WindowId get_id(void) const
            {
                return this->id_;
            }

            std::shared_ptr<wchar_t[]> get_buffer(void)
            {
                return this->buffer_;
            }

            void draw(void)
            {
                // Top and bottom sides
                for(int i = 1; i < this->size_.width - 1; ++i)
                {
                    this->buffer_[i] = U_BAR_HORIZONTAL;
                    this->buffer_[this->size_.width * (this->size_.height - 1) + i] = U_BAR_HORIZONTAL;
                }
                // Left and right sides
                for(int i = 1; i < this->size_.height - 1; ++i)
                {
                    this->buffer_[i * this->size_.width] = U_BAR_VERTICAL;
                    this->buffer_[i * this->size_.width + (this->size_.width - 1)] = U_BAR_VERTICAL;
                }
                // Corners
                this->buffer_[0] = U_CRN_TOP_LEFT;
                this->buffer_[this->size_.width - 1] = U_CRN_TOP_RIGHT;
                this->buffer_[(this->size_.height - 1) * this->size_.width] = U_CRN_BOTTOM_LEFT;
                this->buffer_[(this->size_.height - 1) * this->size_.width + (this->size_.width - 1)] = U_CRN_BOTTOM_RIGHT;
            }

            void set_cursor_pos(int x, int y)
            {
                this->cur_x_ = x;
                this->cur_y_ = y;
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
            int                         cur_x_;
            int                         cur_y_;
            std::shared_ptr<wchar_t[]>  buffer_;
            WindowSize                  size_;
            WindowId                    id_;
            std::vector<WindowId>       children_;
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

            void write(const std::string&& str)
            {
                for(int i = 0; i < str.length(); ++i)
                    this->write_char(str[i]);    
            }

            void write_char(const char c)
            {
                static int wordBeginIdx = 0;

                if(c == ' ')
                    wordBeginIdx = 1;

                if(this->cur_x_ == this->size_.width - 1)
                {
                    int oldWordBeginIdx = wordBeginIdx;
                    int cursorTmp = 1;

                    for(; wordBeginIdx < 0; ++wordBeginIdx)
                    {
                        this->buffer_[(this->cur_y_ + 1) * this->size_.width + cursorTmp] =
                            this->buffer_[(this->cur_y_) * this->size_.width + this->cur_x_ + wordBeginIdx];
                        this->buffer_[(this->cur_y_) * this->size_.width + this->cur_x_ + wordBeginIdx] = ' ';

                        ++cursorTmp;
                    }

                    this->cur_x_ = cursorTmp;
                    ++this->cur_y_;
                }

                this->buffer_[(this->cur_y_) * this->size_.width + this->cur_x_] = c;

                --wordBeginIdx;
                ++this->cur_x_;
            }

        private:

    };
}

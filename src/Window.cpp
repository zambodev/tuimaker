#include <cassert>
#include <cmath>
#include "../headers/Window.h"
#include "../headers/WindowManager.h"
#include "../headers/Utils.h"


namespace tmk
{
    Window::Window(WindowSize wsize)
    : size_(wsize),
      buffer_(std::make_shared<wchar_t[]>(wsize.width * wsize.height)),
      id_(Utils::get_progressive_id()),
      cur_x_(1), cur_y_(1)
    {
        for(int i = 0; i < wsize.width * wsize.height; ++i)
            this->buffer_[i] = U_SPACE;

        this->draw();
    }

    Window::~Window()
    {
    }

    bool Window::operator==(Window &window)
    {
        if(this->id_ == window.get_id())
            return true;
        else
            return false;
    }

    const WindowSize &Window::get_size(void) const
    {
        return this->size_;
    }

    int Window::get_id(void) const
    {
        return this->id_;
    }

    wchar_t Window::get_char_at(int x, int y) const
    {
        return this->buffer_[y * this->size_.width + x];
    }

    std::shared_ptr<wchar_t[]> Window::get_buffer(void)
    {
        return this->buffer_;
    }

    void Window::draw(void)
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

    void Window::set_cursor_pos(int x, int y)
    {
        this->cur_x_ = x;
        this->cur_y_ = y;
    }

    void Window::write(const std::string&& str)
    {
        for(int i = 0; i < str.length(); ++i)
            this->write_char(str[i]);    
    }

    void Window::write_char(const char c)
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
}


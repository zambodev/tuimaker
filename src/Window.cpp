#include <cassert>
#include <cmath>
#include "Window.hpp"
#include "WindowManager.hpp"
#include "Utils.hpp"


namespace tmk
{
    Window::Window(WindowSize wsize)
    : size(wsize),
      buffer(std::make_shared<wchar_t[]>(wsize.width * wsize.height)),
      id(Utils::GetProgressiveId()),
      cursorX(1), cursorY(1)
    {
        for(int i = 0; i < wsize.width * wsize.height; ++i)
            this->buffer[i] = U_SPACE;

        this->Draw();
    }

    Window::~Window()
    {
    }

    bool Window::operator==(Window &window)
    {
        if(this->id == window.GetId())
            return true;
        else
            return false;
    }

    const WindowSize &Window::GetSize(void) const
    {
        return this->size;
    }

    int Window::GetId(void) const
    {
        return this->id;
    }

    wchar_t Window::GetCharAt(int x, int y) const
    {
        return this->buffer[y * this->size.width + x];
    }

    std::shared_ptr<wchar_t[]> Window::GetBuffer(void)
    {
        return this->buffer;
    }

    void Window::Draw(void)
    {
        // Top and bottom sides
        for(int i = 1; i < this->size.width - 1; ++i)
        {
            this->buffer[i] = U_BAR_HORIZONTAL;
            this->buffer[this->size.width * (this->size.height - 1) + i] = U_BAR_HORIZONTAL;
        }
        // Left and right sides
        for(int i = 1; i < this->size.height - 1; ++i)
        {
            this->buffer[i * this->size.width] = U_BAR_VERTICAL;
            this->buffer[i * this->size.width + (this->size.width - 1)] = U_BAR_VERTICAL;
        }
        // Corners
        this->buffer[0] = U_CRN_TOP_LEFT;
        this->buffer[this->size.width - 1] = U_CRN_TOP_RIGHT;
        this->buffer[(this->size.height - 1) * this->size.width] = U_CRN_BOTTOM_LEFT;
        this->buffer[(this->size.height - 1) * this->size.width + (this->size.width - 1)] = U_CRN_BOTTOM_RIGHT;
    }

    void Window::SetCursorPos(int x, int y)
    {
        this->cursorX = x;
        this->cursorY = y;
    }

    void Window::Write(const std::string&& str)
    {
        for(int i = 0; i < str.length(); ++i)
            this->WriteChar(str[i]);    
    }

    void Window::WriteChar(const char c)
    {
        static int wordBeginIdx = 0;

        if(c == ' ')
            wordBeginIdx = 1;

        if(this->cursorX == this->size.width - 1)
        {
            int oldWordBeginIdx = wordBeginIdx;
            int cursorTmp = 1;

            for(; wordBeginIdx < 0; ++wordBeginIdx)
            {
                this->buffer[(this->cursorY + 1) * this->size.width + cursorTmp] =
                    this->buffer[(this->cursorY) * this->size.width + this->cursorX + wordBeginIdx];
                this->buffer[(this->cursorY) * this->size.width + this->cursorX + wordBeginIdx] = ' ';

                ++cursorTmp;
            }

            this->cursorX = cursorTmp;
            ++this->cursorY;
        }

        this->buffer[(this->cursorY) * this->size.width + this->cursorX] = c;

        --wordBeginIdx;
        ++this->cursorX;
    }
}


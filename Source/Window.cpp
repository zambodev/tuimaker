#include "../Include/Window.hpp"
#include "../Include/WindowManager.hpp"
#include "../Include/Utils.hpp"


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

    void Window::Write(int x, int y, const std::string&& str, int lineLength)
    {
        int lineFill = 0;

        if(lineLength > 0 && lineLength > this->size.width - x - 1)
            lineLength = this->size.width - x - 1;

        for(int i = 0; i < str.length(); ++i)
        {
            // Check for end of window
            if(this->buffer[y * this->size.width + x + i] != U_SPACE || (lineLength > 0 && lineFill > 0 && lineFill == lineLength))
            {
                //std::wcout << x << " | " << i << " | " << i % lineLength << "\n" << std::flush;

                int oldX = x;
                int oldY = y;

                x = (lineLength > 0) ? x -= lineLength : 1 - i;
                y += 1;

                if(str[i] != ' ')
                {
                    int idx = i;
                    int xCopy = oldX + i + 1;

                    for(; idx >= 0; --idx)
                    {
                        const char& c = str[idx];
                        
                        if(c == ' ')
                            break;
                        else if(xCopy == 0)
                            break;

                        --xCopy;
                    }

                    if(xCopy > 0 && oldX <= xCopy)
                    {
                        int len = i - idx - 1;

                        for(int j = 0; j < len; ++j)
                        {
                            this->buffer[oldY * this->size.width + oldX + i - len + j] = U_BAR_VERTICAL;
                            this->buffer[y * this->size.width + x + i + j] = str[i - len + j];
                        }

                        x += len;
                        lineFill = len;
                    }
                }
            }
            
            if(lineFill == lineLength)
                lineFill = 0;

            if(lineFill == 0)
            {
                while(str[i] == ' ')
                {
                    ++i;
                    --x;
                }
            }

            this->buffer[y * this->size.width + x + i] = str[i];
            ++lineFill;
        }

    }
}

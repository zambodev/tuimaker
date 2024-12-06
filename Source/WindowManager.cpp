#include <vector>
#include <tuple>
#include <cstring>
#include "../Include/WindowManager.hpp"
#include "../Include/Utils.hpp"


namespace tmk
{
    WindowManager::WindowManager()
        : width(Utils::GetTerminalWidth()), height(Utils::GetTerminalHeight())
    {
        this->buffer = new wchar_t[this->width * this->height];
        this->visibilityMask = new unsigned char[this->width * this->height];
        
        for(unsigned int i = 0; i < this->width * this->height; ++i)
        {
            this->buffer[i] = U_SPACE;
            this->visibilityMask[i] = 0;
        }

    }

    WindowManager::~WindowManager()
    {
        delete this->buffer;
    }

    int WindowManager::GetIndexOf(Window& window)
    {
        int idx = 0;
        std::vector<Window*>::iterator itr;

        for(itr = this->windows.begin(); itr != this->windows.end(); ++itr)
        {
            if(*(*itr) == window)
                return idx;

            ++idx;
        }

        return 0;
    }

    WindowId WindowManager::AddWindow(Window* window)
    {
        this->windows.emplace_back(window);
        auto size = window->GetSize();

        for(int i = 0; i < size.height; ++i)
            for(int j = 0; j < size.width; ++j)
                this->visibilityMask[(size.y + i) * this->width + (size.x + j)] = (unsigned char)this->windows.size();
    
        return window->GetId();
    }

    void WindowManager::RemoveWindow(int id)
    {

    }

    void WindowManager::Render(void)
    {
        int depth;

        for(int i = 0; i < this->height; ++i)
        {
            for(int j = 0; j < this->width; ++j)
            {
                if((depth = this->visibilityMask[i * this->width + j]) == 0)
                    continue;

                Window* window = this->windows.at(depth - 1);
                const WindowSize& size = window->GetSize();

                this->buffer[i * this->width + j] = window->GetBuffer()[(i - size.y) * size.width + (j - size.x)];
            }
        }

        for(unsigned int i = 0; i < this->width * this->height; ++i)
            std::wcout << this->buffer[i];
    }
}

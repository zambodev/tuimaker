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
        this->visibilityMask = new WindowId[this->width * this->height];

        for(unsigned int i = 0; i < this->width * this->height; ++i)
        {
            this->buffer[i] = U_SPACE;
            this->visibilityMask[i] = -1;
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

        this->SetVisible(window->GetId());

        return window->GetId();
    }

    void WindowManager::RemoveWindow(WindowId id)
    {
        for(auto itr = this->windows.begin(); itr < this->windows.end(); ++itr)
            if((*itr)->GetId() == id)
                this->windows.erase(itr);

        this->RefreshVisibilityMask();
    }

    void WindowManager::Render(void)
    {
        for(int i = 0; i < this->height; ++i)
        {
            for(int j = 0; j < this->width; ++j)
            {
                const Window* window = this->GetWindow(this->visibilityMask[i * this->width + j]);
                if(window == nullptr)
                    continue;
                const WindowSize size = window->GetSize();
                this->buffer[i * this->width + j] = window->GetCharAt(j - size.x, i - size.y);
            }
        }

        std::wcout << this->buffer;
    }

    Window* WindowManager::GetWindow(WindowId id)
    {
        for(auto window : this->windows)
            if(window->GetId() == id)
                return window;

        return nullptr;
    }

    void WindowManager::SetVisible(WindowId id)
    {
        Window* window = this->GetWindow(id);

        if(window == nullptr)
            return;

        const WindowSize size = window->GetSize();

        for(int i = 0; i < size.height; ++i)
            for(int j = 0; j < size.width; ++j)
                this->visibilityMask[(size.y + i) * this->width + (size.x + j)] = window->GetId();
    }

    void WindowManager::RefreshVisibilityMask(void)
    {
        memset(this->visibilityMask, -1, this->width * this->height * sizeof(WindowId));

        for(auto itr = this->windows.rbegin(); itr < this->windows.rend(); ++itr)
        {
            const WindowSize size = (*itr)->GetSize();
            const WindowId id = (*itr)->GetId();

            for(int i = 0; i < size.height; ++i)
                for(int j = 0; j < size.width; ++j)
                    if(this->visibilityMask[(size.y + i) * (size.x + size.width) + (size.x + j)] == -1)
                        this->visibilityMask[(size.y + i) * (size.x + size.width) + (size.x + j)] = id;
        }
    }
}

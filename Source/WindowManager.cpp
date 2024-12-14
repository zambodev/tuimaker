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

        for (unsigned int i = 0; i < this->width * this->height; ++i)
        {
            this->buffer[i] = U_SPACE;
            this->visibilityMask[i] = -1;
        }
    }

    WindowManager::~WindowManager()
    {
        delete this->buffer;
    }

    int WindowManager::GetIndexOf(Window &window)
    {
        int idx = 0;
        std::vector<Window *>::iterator itr;

        for (itr = this->windows.begin(); itr != this->windows.end(); ++itr)
        {
            if (*(*itr) == window)
                return idx;

            ++idx;
        }

        return 0;
    }

    WindowId WindowManager::AddWindow(Window *window)
    {
        this->windows.emplace_back(window);
        auto size = window->GetSize();

        this->SetVisible(window->GetId());

        return window->GetId();
    }

    void WindowManager::RemoveWindow(WindowId id)
    {
        for (auto itr = this->windows.begin(); itr < this->windows.end(); ++itr)
            if ((*itr)->GetId() == id)
                this->windows.erase(itr);

        this->RefreshVisibilityMask();
    }

    void WindowManager::Render(void)
    {
        for (int i = 0; i < this->height; ++i)
        {
            for (int j = 0; j < this->width; ++j)
            {
                const Window* window = this->GetWindow(this->visibilityMask[i * this->width + j]);
    
                if (window == nullptr)
                    continue;
    
                const WindowSize size = window->GetSize();

                const BorderType winBorderType = window->IsOnBorder(j, i);

                if(winBorderType != BORDER_TYPE_NONE)
                {
                    BorderType borders[4] = {BORDER_TYPE_NONE};

                    // Get near cells information like if it is a border and which border type
                    if(i > 0 && (winBorderType == BORDER_TYPE_CORNER_TOP_LEFT || winBorderType == BORDER_TYPE_CORNER_TOP_RIGHT))
                    {
                        const Window* win = this->GetWindow(this->visibilityMask[(i - 1) * this->width + j]);
                        if(win != nullptr && win != window)
                            borders[0] = win->IsOnBorder(j, i - 1);
                    }
                    if(i < this->height - 1 && (winBorderType == BORDER_TYPE_CORNER_BOTTOM_LEFT || winBorderType == BORDER_TYPE_CORNER_BOTTOM_RIGHT))
                    {
                        const Window* win = this->GetWindow(this->visibilityMask[(i + 1) * this->width + j]);
                        if(win != nullptr && win != window)
                            borders[1] = win->IsOnBorder(j, i + 1);
                    }
                    if(j > 0 && (winBorderType == BORDER_TYPE_CORNER_TOP_LEFT || winBorderType == BORDER_TYPE_CORNER_BOTTOM_LEFT))
                    {
                        const Window* win = this->GetWindow(this->visibilityMask[i * this->width + (j - 1)]);
                        if(win != nullptr && win != window)
                            borders[2] = win->IsOnBorder(j - 1, i);
                    }
                    if(j < this->width - 1 && (winBorderType == BORDER_TYPE_CORNER_TOP_RIGHT || winBorderType == BORDER_TYPE_CORNER_BOTTOM_RIGHT))
                    {
                        const Window* win = this->GetWindow(this->visibilityMask[i * this->width + (j + 1)]);
                        if(win != nullptr && win != window)
                            borders[3] = win->IsOnBorder(j + 1, i);
                    }

                    // Corners
                    if(winBorderType <= BORDER_TYPE_BOTTOM)
                    {
                        this->buffer[i * this->width + j] = window->GetCharAt(j - size.x, i - size.y);
                    }
                    if(winBorderType > BORDER_TYPE_BOTTOM)
                    {
                        switch(winBorderType)
                        {
                            case BORDER_TYPE_CORNER_TOP_LEFT:
                            {
                                if(borders[0] != BORDER_TYPE_NONE && borders[2] != BORDER_TYPE_NONE)
                                    this->buffer[i * this->width + j] = U_CROSS;
                                else if(borders[0] != BORDER_TYPE_NONE)
                                    this->buffer[i * this->width + j] = U_T_RIGHT;
                                else if(borders[2] != BORDER_TYPE_NONE)
                                    this->buffer[i * this->width + j] = U_T_TOP;
                                else
                                    this->buffer[i * this->width + j] = window->GetCharAt(j - size.x, i - size.y);
                            }
                            break;
                            case BORDER_TYPE_CORNER_TOP_RIGHT:
                            {
                                if(borders[0] != BORDER_TYPE_NONE && borders[3] != BORDER_TYPE_NONE)
                                    this->buffer[i * this->width + j] = U_CROSS;
                                else if(borders[0] != BORDER_TYPE_NONE)
                                    this->buffer[i * this->width + j] = U_T_LEFT;
                                else if(borders[3] != BORDER_TYPE_NONE)
                                    this->buffer[i * this->width + j] = U_T_TOP;
                                else
                                    this->buffer[i * this->width + j] = window->GetCharAt(j - size.x, i - size.y);
                            }
                            break;
                            case BORDER_TYPE_CORNER_BOTTOM_LEFT:
                            {
                                if(borders[1] != BORDER_TYPE_NONE && borders[2] != BORDER_TYPE_NONE)
                                    this->buffer[i * this->width + j] = U_CROSS;
                                else if(borders[1] != BORDER_TYPE_NONE)
                                    this->buffer[i * this->width + j] = U_T_LEFT;
                                else if(borders[2] != BORDER_TYPE_NONE)
                                    this->buffer[i * this->width + j] = U_T_BOTTOM;
                                else
                                    this->buffer[i * this->width + j] = window->GetCharAt(j - size.x, i - size.y);
                            }
                            break;
                            case BORDER_TYPE_CORNER_BOTTOM_RIGHT:
                            {
                                if(borders[1] != BORDER_TYPE_NONE && borders[3] != BORDER_TYPE_NONE)
                                    this->buffer[i * this->width + j] = U_CROSS;
                                else if(borders[1] != BORDER_TYPE_NONE)
                                    this->buffer[i * this->width + j] = U_T_LEFT;
                                else if(borders[3] != BORDER_TYPE_NONE)
                                    this->buffer[i * this->width + j] = U_T_BOTTOM;
                                else
                                    this->buffer[i * this->width + j] = window->GetCharAt(j - size.x, i - size.y);
                            }
                            break;
                        }
                    }
                    // Neither corner nor border
                    else
                    {
                        this->buffer[i * this->width + j] = window->GetCharAt(j - size.x, i - size.y);
                    }
                }
                else
                {
                    this->buffer[i * this->width + j] = window->GetCharAt(j - size.x, i - size.y);
                }
            }
        }

        std::wcout << this->buffer;
    }

    Window *WindowManager::GetWindow(WindowId id)
    {
        for (auto window : this->windows)
            if (window->GetId() == id)
                return window;

        return nullptr;
    }

    void WindowManager::SetVisible(WindowId id)
    {
        Window *window = this->GetWindow(id);

        if (window == nullptr)
            return;

        const WindowSize size = window->GetSize();

        for (int i = 0; i < size.height; ++i)
            for (int j = 0; j < size.width; ++j)
                this->visibilityMask[(size.y + i) * this->width + (size.x + j)] = window->GetId();
    }

    void WindowManager::RefreshVisibilityMask(void)
    {
        memset(this->visibilityMask, -1, this->width * this->height * sizeof(WindowId));

        for (auto itr = this->windows.rbegin(); itr < this->windows.rend(); ++itr)
        {
            const WindowSize size = (*itr)->GetSize();
            const WindowId id = (*itr)->GetId();

            for (int i = 0; i < size.height; ++i)
                for (int j = 0; j < size.width; ++j)
                    if (this->visibilityMask[(size.y + i) * (size.x + size.width) + (size.x + j)] == -1)
                        this->visibilityMask[(size.y + i) * (size.x + size.width) + (size.x + j)] = id;
        }
    }
}

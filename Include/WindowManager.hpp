#pragma once

#include <vector>
#include "Window.hpp"

namespace tmk
{
    class WindowManager
    {
        private:
            int width;
            int height;
            wchar_t* buffer;
            WindowId* visibilityMask;
            std::vector<Window*> windows;
            WindowManager();
            ~WindowManager();

        public:
            WindowManager(const WindowManager& obj) = delete;

            static WindowManager* GetInstance(void)
            {
                static WindowManager* instance = new WindowManager();
                return instance;
            }

            int GetIndexOf(Window& window);
            WindowId AddWindow(Window* window);
            void RemoveWindow(WindowId id);
            void Render(void);
            Window* GetWindow(WindowId id);
            void SetVisible(WindowId id);
            void RefreshVisibilityMask(void);
    };
}

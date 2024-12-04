#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <vector>
#include "Window.hpp"


class WindowManager
{
    private:
        int m_Width;
        int m_Height;
        wchar_t* m_Buffer;
        int* m_BufferLayerMap;
        std::vector<Window*> m_VisibilityLayerList;
        WindowManager();
        ~WindowManager();

    public:
        WindowManager(const WindowManager& obj) = delete;

        static WindowManager* getInstance(void)
        {
            static WindowManager* instance = new WindowManager();
            return instance;
        }

        int getIndexOf(Window& window);
        void addWindow(Window* window);
        void removeWindow(int id);
        void render(void);
};

#endif

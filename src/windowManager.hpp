#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <vector>
#include "window.hpp"


class WindowManager
{
    private:
        int m_Width;
        int m_Height;
        wchar_t* m_Buffer;
        int* m_BufferLayerMap;
        std::vector<Window*> m_visibilityLayerList;

    public:
        WindowManager();
        ~WindowManager();

        int getIndexOf(Window& window);
        void addWindow(Window& window);
        void removeWindow(int id);
        void render(void);
};

#endif

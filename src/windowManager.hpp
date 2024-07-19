#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <vector>
#include "window.hpp"


class WindowManager
{
    private:
        wchar_t* m_Buffer;
        int* m_BufferLayerMap;
        std::vector<std::vector<Window*>*> m_visibilityLayerList;

    public:
        WindowManager();
        ~WindowManager();

        void addWindow(Window& window);
        void removeWindow(int id);
        void render(void);
};

#endif

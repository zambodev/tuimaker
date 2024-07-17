#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <vector>
#include "window.hpp"


class WindowManager
{
    private:
        std::vector<std::vector<Window*>*> m_visibilityLayerList;

    public:
        void addWindow(Window& window);
        void removeWindow(Window& window);

};

#endif

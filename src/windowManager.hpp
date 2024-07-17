#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <list>
#include "window.hpp"


class WindowManager
{
    private:
        std::list<std::list<Window&>> visibilityLayerList;

    public:
        void addWindow(Window& window);
        void removeWindow(Window& window);

};

#endif

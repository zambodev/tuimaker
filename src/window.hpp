#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <iostream>
#include <list>
#include "box.hpp"


class Window : protected Box
{
    private:
        std::list<Window*> m_nearbyWindows;

    public:
        Window(const int&& x, const int&& y, const int&& width, const int&& height);
        Window(const int&& x, const int&& y, const int&& width, const int&& height,
               const unsigned short&& cornerBitmask, Window& fatherWindow);
        ~Window();

        void addChild(Window* window);
        unsigned short checkCornerCollision(const int&& x, const int&& y, const int&& width, const int&& height);
        void show(void);
};

#endif

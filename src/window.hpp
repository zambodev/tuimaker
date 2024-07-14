#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <iostream>
#include <list>
#include "box.hpp"


class Window : protected Box
{
    private:
        Window* m_fatherWindow;
        std::list<Window> m_childrenWindows;

    public:
        Window(const int&& x, const int&& y, const int&& width, const int&& height);
        Window(const int&& x, const int&& y, const int&& width, const int&& height,
               const unsigned char&& cornerBitmask, Window& fatherWindow);
        ~Window();
};

#endif

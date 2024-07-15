#include <array>
#include "window.hpp"


Window::Window(const int&& x, const int&& y, const int&& width, const int&& height)
    : Box(std::move(x), std::move(y), std::move(width), std::move(height), 0x00)
{
    std::wcout << L"Created standalone Window!\n";

    this->addChild(this);
}

Window::Window(const int&& x, const int&& y, const int&& width, const int&& height,
               const unsigned short&& cornerBitmask, Window& fatherWindow) 
    : Box(std::move(x), std::move(y), std::move(width), std::move(height), 0)
{
    std::wcout << L"Created child Window!\n";

    fatherWindow.addChild(this);
    this->setBitmask(fatherWindow.checkCornerCollision(std::move(x), std::move(y), std::move(width), std::move(height)));
}

Window::~Window()
{
}

void Window::addChild(Window* window)
{
    this->m_nearbyWindows.emplace_back(window);
}


unsigned short Window::checkCornerCollision(const int&& x, const int&& y, const int&& width, const int&& height)
{
    unsigned short bitmask = 0x00;

    std::wcout << L"\x1b[0;0H";

    for(Window* w : this->m_nearbyWindows)
    {
        /* ADD CHECK TO DIFFERENT WINDOW THAN THIS ONE */

        std::array<int, 4> size = w->getSize();

        // Left corner T top
        if(x > size[0] && x < (size[0] + size[2] - 1) && y == size[1])
        {
            bitmask ^= (bitmask & B_CRN_TOP_LEFT);
            bitmask |= V_CRN_T_TOP;
        }
        // Left corner T left
        if(x == size[0] && y > size[1] && y < (size[1] + size[3] - 1))
        {
            bitmask ^= (bitmask & B_CRN_TOP_LEFT);
            bitmask |= V_CRN_T_LEFT;
        }
        // Right corner T left
        if(x == (size[0] + size[2] - 1) && y >= size[1] && y <= (size[1] + size[3] - 1))
        {
            bitmask ^= (bitmask & B_CRN_TOP_LEFT);
            bitmask |= V_CRN_T_RIGHT;
        }

        std::wcout << size[0] << size[1] << size[2] << size[3];
    }

    std::wcout << bitmask;

    return bitmask;
}

void Window::show(void)
{
    this->draw();
}

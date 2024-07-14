#include "window.hpp"


Window::Window(const int&& x, const int&& y, const int&& width, const int&& height)
    : Box(std::move(x), std::move(y), std::move(width), std::move(height), 0)
{
    std::wcout << L"Created standalone Window!\n";
}
Window::Window(const int&& x, const int&& y, const int&& width, const int&& height,
               const unsigned char&& cornerBitmask, Window& fatherWindow) 
    : Box(std::move(x), std::move(y), std::move(width), std::move(height), std::move(cornerBitmask)),
      m_fatherWindow(&fatherWindow)
{
    std::wcout << L"Created child Window!\n";
}

Window::~Window()
{
}

#include "window.hpp"


Window::Window(int x, int y, int width, int height) 
    : Box(x, y, width, height)
{
    std::wcout << L"Creating Window!\n";
}

Window::~Window()
{
}

void Window::setWidth(int width)
{
    this->m_Width = width;
}

int Window::getWidth(void)
{
    return this->m_Width;
}

void Window::setHeight(int height)
{
    this->m_Height = height;
}

int Window::getHeight(void)
{
    return this->m_Height;
}

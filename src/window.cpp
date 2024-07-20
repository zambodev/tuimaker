#include "window.hpp"
#include "utils.hpp"


Window::Window(const int&& x, const int&& y, const int&& width, const int&& height,
               const unsigned short&& cornerBitmask, Window* father)
    : m_Size({x, y, width, height}),
      m_Buffer(new wchar_t[width * height]), m_Father(father),
      m_Id(UTILS::getProgressiveId()), m_Selectable(false),
      m_Selected(false), m_Writable(false), m_Ready(false)
{
    for(int i = 0; i < this->m_Size.width * this->m_Size.height; ++i)
        this->m_Buffer[i] = U_SPACE;

    if(father == NULL)
        std::wcout << L"Created Window!\n";
    else
        std::wcout << L"Created child Window!\n";
}

Window::~Window()
{
    delete this->m_Buffer;
}

bool Window::operator==(Window& window)
{
    if(this->m_Id == window.getId())
        return true;
    else
        return false;
}

WindowSize_t& Window::getSize(void)
{
    return m_Size;
}

Window* Window::getFather(void)
{
    return this->m_Father;
}

void Window::setSelected(bool isSelected)
{
    this->m_Selected = isSelected;
}

bool Window::isSelected(void)
{
    return this->m_Selected;
}

void Window::setSelectable(bool isSelectable)
{
    this->m_Selectable = isSelectable;
}

bool Window::isSelectable(void)
{
    return this->m_Selectable;
}

void Window::setWritable(bool isWritable)
{
    this->m_Writable = isWritable;
}

bool Window::isWritable(void)
{
    return this->m_Writable;
}

bool Window::isReady(void)
{
    return this->m_Ready;
}

int Window::getId(void)
{
    return this->m_Id;
}

wchar_t* Window::getBuffer(void)
{
    return this->m_Buffer;
}

void Window::draw(void)
{
    // Top and bottom sides
    for(int i = 1; i < this->m_Size.width - 1; ++i)
    {
        this->m_Buffer[i] = U_BAR_HORIZONTAL;
        this->m_Buffer[this->m_Size.width * (this->m_Size.height - 1) + i] = U_BAR_HORIZONTAL;
    }
    // Left and right sides
    for(int i = 1; i < this->m_Size.height - 1; ++i)
    {
        this->m_Buffer[i * this->m_Size.width] = U_BAR_VERTICAL;
        this->m_Buffer[i * this->m_Size.width + (this->m_Size.width - 1)] = U_BAR_VERTICAL;
    }
    // Corners
    this->m_Buffer[0] = U_CRN_TOP_LEFT;
    this->m_Buffer[this->m_Size.width - 1] = U_CRN_TOP_RIGHT;
    this->m_Buffer[(this->m_Size.height - 1) * this->m_Size.width] = U_CRN_BOTTOM_LEFT;
    this->m_Buffer[(this->m_Size.height - 1) * this->m_Size.width + (this->m_Size.width - 1)] = U_CRN_BOTTOM_RIGHT;

    this->m_Ready = true;
}


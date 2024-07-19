#include "window.hpp"
#include "utils.hpp"


wchar_t Window::getTCorner(unsigned short&& val)
{
    wchar_t uCorner = 0;

    switch (val)
    {
        case V_CRN_T_TOP:
            uCorner = U_T_TOP;
            break;
        case V_CRN_T_BOTTOM:
            uCorner = U_T_BOTTOM;
            break;
        case V_CRN_T_LEFT:
            uCorner = U_T_LEFT;
            break;
        case V_CRN_T_RIGHT:
            uCorner = U_T_RIGHT;
            break;
        case V_CROSS:
            uCorner = U_CROSS;
            break;
        default:
            break;
    }

    return uCorner;
}

Window::Window(const int&& x, const int&& y, const int&& width, const int&& height,
               const unsigned short&& cornerBitmask, Window* father)
    : m_X(x), m_Y(y), m_Width(width), m_Height(height),
      m_Buffer(new wchar_t[width * height]), m_CornerBitmask(cornerBitmask),
      m_father(father), m_id(UTILS::getProgressiveId()),
      m_Selectable(false), m_Selected(false), m_Writable(false), m_Ready(false)
{
    for(int i = 0; i < this->m_Width * this->m_Height; ++i)
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

std::array<int, 4> Window::getSize(void)
{
    return {this->m_X, this->m_Y, this->m_Width, this->m_Height};
}

Window* Window::getFather(void)
{
    return this->m_father;
}

void Window::setBitmask(const unsigned short && bitmask)
{
    this->m_CornerBitmask = bitmask;
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

void Window::draw(void)
{
    // Top and bottom sides
    for(int i = 1; i < this->m_Width - 1; ++i)
    {
        this->m_Buffer[i] = U_BAR_HORIZONTAL;
        this->m_Buffer[this->m_Width * (this->m_Height - 1) + i] = U_BAR_HORIZONTAL;
    }
    // Left and right sides
    for(int i = 1; i < this->m_Height - 1; ++i)
    {
        this->m_Buffer[i * this->m_Width] = U_BAR_VERTICAL;
        this->m_Buffer[i * this->m_Width + (this->m_Width - 1)] = U_BAR_VERTICAL;
    }
    // Corners
    wchar_t corner;
    if((corner = getTCorner(this->m_CornerBitmask & B_CRN_TOP_LEFT)) != 0)
        this->m_Buffer[0] = corner;
    else
        this->m_Buffer[0] = U_CRN_TOP_LEFT;
    this->m_Buffer[this->m_Width - 1] = U_CRN_TOP_RIGHT;
    this->m_Buffer[(this->m_Height - 1) * this->m_Width] = U_CRN_BOTTOM_LEFT;
    this->m_Buffer[(this->m_Height - 1) * this->m_Width + (this->m_Width - 1)] = U_CRN_BOTTOM_RIGHT;

    this->m_Ready = true;
}

void Window::show(wchar_t* buffer, int* layerMap)
{
    auto size = UTILS::getTerminalSize();
    int depth = UTILS::getWindowDepth(this);

    for(int i = 0; i < this->m_Height; ++i)
        for(int j = 0; j < this->m_Width; ++j)
            if(layerMap[(this->m_Y + i) * std::get<0>(size) + (this->m_X + j)] == depth)
                buffer[(this->m_Y + i) * std::get<0>(size) + (this->m_X + j)] = this->m_Buffer[i * this->m_Width + j];
}

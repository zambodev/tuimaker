#include "box.hpp"


Box::Box(const int&& x, const int&& y, const int&& width, const int&& height,
         const unsigned char&& cornerBitmask)
    : m_X(x), m_Y(y), m_Width(width), m_Height(height),
      m_Buffer(new wchar_t[width * height]), m_CornerBitmask(cornerBitmask)
{
    for(int i = 0; i < width * height; ++i)
        this->m_Buffer[i] = U_SPACE;

    this->draw();

    std::wcout << L"Created Box!\n";
}

Box::~Box()
{
    delete this->m_Buffer;
}

void Box::setSelected(bool isSelected)
{
    this->m_Selected = isSelected;
}

bool Box::isSelected(void)
{
    return this->m_Selected;
}

void Box::setSelectable(bool isSelectable)
{
    this->m_Selectable = isSelectable;
}

bool Box::isSelectable(void)
{
    return this->m_Selectable;
}

void Box::setWritable(bool isWritable)
{
    this->m_Writable = isWritable;
}

bool Box::isWritable(void)
{
    return this->m_Writable;
}

void Box::draw(void)
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
    this->m_Buffer[0] = U_CORNER_TOP_LEFT;
    this->m_Buffer[this->m_Width - 1] = U_CORNER_TOP_RIGHT;
    this->m_Buffer[(this->m_Height - 1) * this->m_Width] = U_CORNER_BOTTOM_LEFT;
    this->m_Buffer[(this->m_Height - 1) * this->m_Width + (this->m_Width - 1)] = U_CORNER_BOTTOM_RIGHT;

    std::wcout << L"\x1b[s\x1b[0;0H"
               << this->m_Buffer
               << L"\x1b[10;0H";
}

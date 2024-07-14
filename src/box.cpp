#include "box.hpp"


Box::Box(const int &x, const int &y, const int &width, const int &height)
    : m_X(x), m_Y(y), m_Width(width), m_Height(height),
      m_Buffer(new wchar_t[width * height])
{
    std::wcout << L"Creating Box!\n";

    for (int i = 0; i < width * height; ++i)
        this->m_Buffer[i] = SPACE;

    this->draw();
}

Box::~Box()
{
    delete this->m_Buffer;
}

void Box::setSelected(bool isSelected)
{
    this->m_selected = isSelected;
}

bool Box::isSelected(void)
{
    return this->m_selected;
}

void Box::setSelectable(bool isSelectable)
{
    this->m_selectable = isSelectable;
}

bool Box::isSelectable(void)
{
    return this->m_selectable;
}

void Box::setWritable(bool isWritable)
{
    this->m_writable = isWritable;
}

bool Box::isWritable(void)
{
    return this->m_writable;
}

void Box::draw(void)
{
    for (int i = 0; i < this->m_Width; ++i)
    {
        this->m_Buffer[i] = HBAR;
        this->m_Buffer[this->m_Width * (this->m_Height - 1) + i] = HBAR;
    }

    std::wcout << L"\x1b[s\x1b[0;0H" << this->m_Buffer << L"\x1b[s\x1b[10;0H";
}

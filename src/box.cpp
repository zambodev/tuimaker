#include "box.hpp"


wchar_t Box::getTCorner(unsigned short&& val)
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

Box::Box(const int&& x, const int&& y, const int&& width, const int&& height,
         const unsigned short&& cornerBitmask)
    : m_X(x), m_Y(y), m_Width(width), m_Height(height),
      m_Buffer(new wchar_t[width * height]), m_CornerBitmask(cornerBitmask)
{
    for(int i = 0; i < this->m_Width * this->m_Height; ++i)
        this->m_Buffer[i] = U_SPACE;

    std::wcout << L"Created Box!\n";
}

Box::~Box()
{
    delete this->m_Buffer;
}

std::array<int, 4> Box::getSize(void)
{
    return {this->m_X, this->m_Y, this->m_Width, this->m_Height};
}

void Box::setBitmask(const unsigned short && bitmask)
{
    this->m_CornerBitmask = bitmask;
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
    wchar_t corner;
    if((corner = getTCorner(this->m_CornerBitmask & B_CRN_TOP_LEFT)) != 0)
        this->m_Buffer[0] = corner;
    else
        this->m_Buffer[0] = U_CRN_TOP_LEFT;
    this->m_Buffer[this->m_Width - 1] = U_CRN_TOP_RIGHT;
    this->m_Buffer[(this->m_Height - 1) * this->m_Width] = U_CRN_BOTTOM_LEFT;
    this->m_Buffer[(this->m_Height - 1) * this->m_Width + (this->m_Width - 1)] = U_CRN_BOTTOM_RIGHT;

    std::wcout << L"\x1b[s\x1b[" << this->m_Y << ";" << this->m_X << "H";
    
    for(int i = 0; i < this->m_Height; ++i)
    {
        for(int j = 0; j < this->m_Width; ++j)
            wprintf(L"%lc", this->m_Buffer[i * this->m_Width + j]);

        if(i < (this->m_Height - 1))
            std::wcout << L"\x1b[" << this->m_Y + i + 1 << ";" << this->m_X << "H";
    }

    std::wcout << L"\x1b[30;0H";
}

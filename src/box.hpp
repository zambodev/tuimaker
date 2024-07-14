#ifndef BOX_HPP
#define BOX_HPP

#include <iostream>


class Box
{
    private:
        unsigned char m_CornerBitmask;

        int m_X;
        int m_Y;
        int m_Width;
        int m_Height;

        bool m_Selectable;
        bool m_Selected;
        bool m_Writable;
        
        wchar_t* m_Buffer;

        enum : wchar_t
        {
            U_BAR_HORIZONTAL        = u'\u2501',
            U_BAR_VERTICAL          = u'\u2503',
            U_CORNER_TOP_LEFT       = u'\u250f',
            U_CORNER_TOP_RIGHT      = u'\u2513',
            U_CORNER_BOTTOM_LEFT    = u'\u2517',
            U_CORNER_BOTTOM_RIGHT   = u'\u251b',
            U_CROSS                 = u'\u254b',
            U_T_LEFT                = u'\u2523',
            U_T_RIGHT               = u'\u252b',
            U_T_TOP                 = u'\u2533',
            U_T_BOTTOM              = u'\u253B',
            U_SPACE                 = u'\u0020'
        };

    protected:
        enum : unsigned char
        {
            B_CORNER_TOP_LEFT       = 0xC0,
            B_CORNER_TOP_RIGHT      = 0x30,
            B_CORNER_BOTTOM_LEFT    = 0x0C,
            B_CORNER_BOTTOM_RIGHT   = 0x03
        };

    public:
        Box(const int&& x, const int&& y, const int&& width, const int&& height,
            const unsigned char&& cornerBitmask);
        ~Box();

        void setSelected(bool isSelected);
        bool isSelected(void);
        void setSelectable(bool isSelectable);
        bool isSelectable(void);
        void setWritable(bool isWritable);
        bool isWritable(void);

        void draw(void);
};

#endif

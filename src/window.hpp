#ifndef BOX_HPP
#define BOX_HPP

#include <iostream>
#include <array>


enum : wchar_t
{
    U_BAR_HORIZONTAL        = u'\u2501',
    U_BAR_VERTICAL          = u'\u2503',
    U_CRN_TOP_LEFT          = u'\u250f',
    U_CRN_TOP_RIGHT         = u'\u2513',
    U_CRN_BOTTOM_LEFT       = u'\u2517',
    U_CRN_BOTTOM_RIGHT      = u'\u251b',
    U_CROSS                 = u'\u254b',
    U_T_LEFT                = u'\u2523',
    U_T_RIGHT               = u'\u252b',
    U_T_TOP                 = u'\u2533',
    U_T_BOTTOM              = u'\u253B',
    U_SPACE                 = u'\u0020'
};

struct WindowSize_t
{
    int x;
    int y;
    int width;
    int height;
};

class Window
{
    private:
        int m_Id;
        bool m_Ready;
        bool m_Selectable;
        bool m_Selected;
        bool m_Writable;
        wchar_t* m_Buffer;
        WindowSize_t m_Size;
        Window* m_Father;

    protected:
        enum : unsigned short
        {
            B_CRN_TOP_LEFT          = 0x000FU,
            B_CRN_TOP_RIGHT         = 0x00F0U,
            B_CRN_BOTTOM_LEFT       = 0x0F00U,
            B_CRN_BOTTOM_RIGHT      = 0xF000U,
        };

        enum : unsigned short
        {
            V_CRN_T_TOP             = 0x1,
            V_CRN_T_BOTTOM          = 0x2,
            V_CRN_T_LEFT            = 0x3,
            V_CRN_T_RIGHT           = 0x4,
            V_CROSS                 = 0x5
        };

    public:
        Window(const int&& x, const int&& y, const int&& width, const int&& height,
            const unsigned short&& cornerBitmask, Window* father);
        ~Window();

        bool operator==(Window& window);

        WindowSize_t& getSize(void);
        Window* getFather(void);
        void setSelected(bool isSelected);
        bool isSelected(void);
        void setSelectable(bool isSelectable);
        bool isSelectable(void);
        void setWritable(bool isWritable);
        bool isWritable(void);
        bool isReady(void);
        int getId(void);
        wchar_t* getBuffer(void);

        void draw(void);
};

#endif

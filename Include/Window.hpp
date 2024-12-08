#pragma once

#include <iostream>
#include <array>

namespace tmk
{
    typedef int WindowId;

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

    struct WindowSize
    {
        int x;
        int y;
        int width;
        int height;
    };

    class Window
    {
        private:
            int id;
            bool isSelectable;
            bool isSelected;
            bool isWritable;
            wchar_t* buffer;
            WindowSize size;
            WindowId father;

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
            Window() = delete;
            Window(const int&& x, const int&& y, const int&& width, const int&& height,
                const unsigned short&& cornerBitmask, WindowId father);
            ~Window();
            // Operators
            bool operator==(Window& window);
            // Functions
            const WindowSize& GetSize(void) const;
            WindowId GetFather(void) const;
            void SetSelected(bool isSelected);
            bool IsSelected(void) const;
            void SetSelectable(bool isSelectable);
            bool IsSelectable(void) const;
            void SetWritable(bool isWritable);
            bool IsWritable(void) const;
            int GetId(void) const;
            wchar_t GetCharAt(int x, int y) const;
            const wchar_t* GetBuffer(void) const;
            void Draw(void);
    };
}

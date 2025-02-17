#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <memory>

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
            int                         cursorX;
            int                         cursorY;
            WindowSize                  size;
            WindowId                    id;
            std::shared_ptr<wchar_t[]>  buffer;

        public:
            Window() = delete;
            Window(WindowSize size);
            ~Window();
            // Operators
            bool operator==(Window& window);
            // Functions
            const WindowSize& GetSize(void) const;
            int GetId(void) const;
            wchar_t GetCharAt(int x, int y) const;
            std::shared_ptr<wchar_t[]> GetBuffer(void);
            void Draw(void);
            void Write(int x, int y, const std::string&& str, int lineLength = 0);
    };
}

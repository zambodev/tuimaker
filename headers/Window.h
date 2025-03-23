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
        public:
            Window() = delete;
            Window(WindowSize size);
            ~Window();
            // Operators
            bool operator==(Window& window);
            // Functions
            const WindowSize& get_size(void) const;
            int get_id(void) const;
            wchar_t get_char_at(int x, int y) const;
            std::shared_ptr<wchar_t[]> get_buffer(void);
            void draw(void);
            void set_cursor_pos(int x, int y);
            void write(const std::string&& str);
            void write_char(const char c);
    
        private:
            int                         cur_x_;
            int                         cur_y_;
            WindowSize                  size_;
            WindowId                    id_;
            std::shared_ptr<wchar_t[]>  buffer_;
    };
}

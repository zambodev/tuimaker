#pragma once

#include <iostream>

namespace tmk
{
    struct TChar
    {
        // Window borders
        static constexpr wchar_t U_BAR_HORIZONTAL = u'\u2501';
        static constexpr wchar_t U_BAR_VERTICAL = u'\u2503';
        static constexpr wchar_t U_CRN_TOP_LEFT = u'\u250f';
        static constexpr wchar_t U_CRN_TOP_RIGHT = u'\u2513';
        static constexpr wchar_t U_CRN_BOTTOM_LEFT = u'\u2517';
        static constexpr wchar_t U_CRN_BOTTOM_RIGHT = u'\u251b';
        static constexpr wchar_t U_CROSS = u'\u254b';
        static constexpr wchar_t U_T_LEFT = u'\u2523';
        static constexpr wchar_t U_T_RIGHT = u'\u252b';
        static constexpr wchar_t U_T_TOP = u'\u2533';
        static constexpr wchar_t U_T_BOTTOM = u'\u253B';
        static constexpr wchar_t U_SPACE = u'\u0020';
        // Text color
        static constexpr const wchar_t *TC_BLACK = L"\e[30m";
        static constexpr const wchar_t *TC_RED = L"\e[31m";
        static constexpr const wchar_t *TC_GREEN = L"\e[32m";
        static constexpr const wchar_t *TC_YELLOW = L"\e[33m";
        static constexpr const wchar_t *TC_BLUE = L"\e[34m";
        static constexpr const wchar_t *TC_MAGENTA = L"\e[35m";
        static constexpr const wchar_t *TC_CYAN = L"\e[36m";
        static constexpr const wchar_t *TC_WHITE = L"\e[37m";
        // Background color
        static constexpr const wchar_t *BGC_BLACK = L"\e[40m";
        static constexpr const wchar_t *BGC_RED = L"\e[41m";
        static constexpr const wchar_t *BGC_GREEN = L"\e[42m";
        static constexpr const wchar_t *BGC_YELLOW = L"\e[43m";
        static constexpr const wchar_t *BGC_BLUE = L"\e[44m";
        static constexpr const wchar_t *BGC_MAGENTA = L"\e[45m";
        static constexpr const wchar_t *BGC_CYAN = L"\e[46m";
        static constexpr const wchar_t *BGC_WHITE = L"\e[47m";

        const wchar_t *text_color;
        const wchar_t *bg_color;
        wchar_t character;

        TChar()
        {
            text_color = TC_WHITE;
            bg_color = BGC_BLACK;
            character = U_SPACE;
        }

        ~TChar()
        {
        }

        TChar &operator=(const TChar &obj)
        {
            text_color = obj.text_color;
            bg_color = obj.text_color;
            character = obj.character;

            return *this;
        }
    };

    std::wostream &operator<<(std::wostream &out, const TChar &obj)
    {
        out << obj.text_color << obj.bg_color << obj.character;

        return out;
    }
}
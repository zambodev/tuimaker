#pragma once

#include <iostream>

namespace tmk
{
    /**
     * @struct TChar
     * @brief Terminal character structure that holds the character
     * the text color and the background color for each cell
     *
     */
    struct TChar
    {
        // Window borders unicode characters
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
        static constexpr const wchar_t *TC_BLACK = L"\x1b[30m";
        static constexpr const wchar_t *TC_RED = L"\x1b[31m";
        static constexpr const wchar_t *TC_GREEN = L"\x1b[32m";
        static constexpr const wchar_t *TC_YELLOW = L"\x1b[33m";
        static constexpr const wchar_t *TC_BLUE = L"\x1b[34m";
        static constexpr const wchar_t *TC_MAGENTA = L"\x1b[35m";
        static constexpr const wchar_t *TC_CYAN = L"\x1b[36m";
        static constexpr const wchar_t *TC_WHITE = L"\x1b[37m";
        static constexpr const wchar_t *TC_DEFAULT = L"\x1b[39m";
        // Background color
        static constexpr const wchar_t *BGC_BLACK = L"\x1b[40m";
        static constexpr const wchar_t *BGC_RED = L"\x1b[41m";
        static constexpr const wchar_t *BGC_GREEN = L"\x1b[42m";
        static constexpr const wchar_t *BGC_YELLOW = L"\x1b[43m";
        static constexpr const wchar_t *BGC_BLUE = L"\x1b[44m";
        static constexpr const wchar_t *BGC_MAGENTA = L"\x1b[45m";
        static constexpr const wchar_t *BGC_CYAN = L"\x1b[46m";
        static constexpr const wchar_t *BGC_WHITE = L"\x1b[47m";
        static constexpr const wchar_t *BGC_DEFAULT = L"\x1b[49m";

        const wchar_t *text_color;
        const wchar_t *bg_color;
        wchar_t character;

        /**
         * @brief Construct a new TChar object
         *
         */
        TChar()
        {
            text_color = TC_DEFAULT;
            bg_color = BGC_DEFAULT;
            character = U_SPACE;
        }

        /**
         * @brief Destroy the TChar object
         *
         */
        ~TChar()
        {
        }

        /**
         * @brief Copy constructor
         *
         * @param obj TChar object
         * @return TChar&
         */
        auto operator=(const TChar &obj) -> TChar &
        {
            text_color = obj.text_color;
            bg_color = obj.bg_color;
            character = obj.character;

            return *this;
        }
    };

    /**
     * @brief Operator<< overload for wide stdout
     *
     * @param out std::wostream
     * @param obj TChar object
     * @return std::wostream&
     */
    auto operator<<(std::wostream &out, const TChar &obj) -> std::wostream &
    {
        out << obj.text_color << obj.bg_color << obj.character;

        return out;
    }
}
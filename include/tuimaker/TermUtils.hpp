#pragma once

#include <tuple>
#include <vector>
#include <fcntl.h>
#ifdef __linux__
#include <sys/ioctl.h>
#include <unistd.h>
#include <clocale>
#elif _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

namespace tmk
{
    /**
     * @brief Terminal utilities namespace
     *
     */
    namespace TermUtils
    {
        /**
         * @brief Get the term size
         *
         * @return std::pair<uint64_t, uint64_t>
         */
        auto get_term_size(void) -> std::pair<uint64_t, uint64_t>
        {
#ifdef __linux__
            setlocale(LC_ALL, "");

            struct winsize w;

            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

            return {w.ws_col, w.ws_row};
#elif _WIN32
            _setmode(_fileno(stdout), _O_U16TEXT);

            CONSOLE_SCREEN_BUFFER_INFO csbi;

            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

            return {(csbi.srWindow.Right - csbi.srWindow.Left + 1),
                    (csbi.srWindow.Bottom - csbi.srWindow.Top + 1)};
#endif
        }

        /**
         * @brief Get the progressive id object
         * @note This should be changed
         * @return uint64_t
         */
        auto get_progressive_id(void) -> uint64_t
        {
            static uint64_t id = 0;
            // Skip id 0
            return ++id;
        }
    }
}

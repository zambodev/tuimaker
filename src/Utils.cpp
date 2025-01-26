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
#include "Utils.hpp"


namespace tmk
{
    namespace Utils
    {
        int GetTerminalWidth(void)
        {
    #ifdef __linux__
            setlocale(LC_ALL, "");

            struct winsize w;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            return w.ws_col;
    #elif _WIN32
            _setmode(_fileno(stdout), _O_U16TEXT);

            CONSOLE_SCREEN_BUFFER_INFO csbi;

            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    #endif
        }

        int GetTerminalHeight(void)
        {
    #ifdef __linux__
            setlocale(LC_ALL, "");

            struct winsize w;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            return w.ws_row;
    #elif _WIN32
            _setmode(_fileno(stdout), _O_U16TEXT);

            CONSOLE_SCREEN_BUFFER_INFO csbi;

            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    #endif
        }

        int GetProgressiveId(void)
        {
            static int id = 0;

            return id++;
        }
    }
}

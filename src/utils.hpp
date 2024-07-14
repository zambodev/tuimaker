#ifndef UTILS_HPP
#define UTILS_HPP

#include <tuple>
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


namespace UTILS
{
    std::tuple<int, int> getTerminalSize(void)
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
        return { csbi.srWindow.Right - csbi.srWindow.Left + 1,
                 csbi.srWindow.Bottom - csbi.srWindow.Top + 1 }
#endif
    }
}

#endif

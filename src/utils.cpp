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
#include "utils.hpp"


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

    int getProgressiveId(void)
    {
        static int id = 0;

        return id++;
    }

    int getWindowDepth(Window* window)
    {
        if(window->getFather() != NULL)
            return getWindowDepth(window->getFather()) + 1;
        else 
            return 0;

    }
}

#include <tuple>
#include <vector>
#include <fcntl.h>
#ifdef __linux__
#include <sys/ioctl.h>
#include <unistd.h>
#include <clocale>
#include <termios.h>
#elif _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif
#include "../headers/Utils.h"


namespace tmk
{
    namespace Utils
    {
        int get_term_width(void)
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

        int get_term_height(void)
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

        int get_progressive_id(void)
        {
            static int id = 0;

            return id++;
        }

        void enable_buff_input(void)
        {
            struct termios t;
            tcgetattr(STDIN_FILENO, &t);
            t.c_lflag &= (~ICANON & ~ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &t);
        }

        void disable_buff_input(void)
        {
            struct termios t;
            tcgetattr(STDIN_FILENO, &t);
            t.c_lflag |= (ICANON & ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &t);
        }
    }
}

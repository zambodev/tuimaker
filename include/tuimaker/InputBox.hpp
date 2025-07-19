#pragma once

#include <chrono>
#include <tuimaker/TextBox.hpp>
#include <tuimaker/Utils.hpp>

namespace tmk
{
    class InputBox : public TextBox
    {
    public:
        InputBox(WindowSize wsize)
            : TextBox(wsize) {
              };

        ~InputBox() {
        };

        void read(void)
        {
            fd_set sigfd;
            struct timeval tv;

            FD_ZERO(&sigfd);
            FD_SET(0, &sigfd);

            tv.tv_sec = 0;
            tv.tv_usec = 10000; // 10ms

            if (!select(1, &sigfd, NULL, NULL, &tv))
                return;

            char c = getchar();

            // Esc key
            if (c == 27)
                return;

            write_char(c);
        }

    private:
        bool is_selected_ = false;
    };
}
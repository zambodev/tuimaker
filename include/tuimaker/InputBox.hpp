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
    };
}
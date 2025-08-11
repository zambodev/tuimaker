#pragma once

#include <chrono>
#include <tuimaker/TextBox.hpp>
#include <tuimaker/TermUtils.hpp>

namespace tmk
{
    class InputBox : public TextBox
    {
    public:
        InputBox(Size wsize)
            : TextBox(wsize) {
              };

        ~InputBox() {
        };
    };
}
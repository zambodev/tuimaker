#pragma once

#include <chrono>
#include <tuimaker/TextBox.hpp>
#include <tuimaker/TermUtils.hpp>

namespace tmk
{
    class InputBox : public TextBox
    {
    public:
        InputBox(Size wsize, const Conf &conf = {true, TChar::TC_DEFAULT, TChar::BGC_DEFAULT})
            : TextBox(wsize, conf) {
              };

        ~InputBox() {
        };
    };
}
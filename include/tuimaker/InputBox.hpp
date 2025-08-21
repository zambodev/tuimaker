#pragma once

#include <chrono>
#include <tuimaker/TextBox.hpp>
#include <tuimaker/TermUtils.hpp>

namespace tmk
{
    class InputBox : public TextBox
    {
    public:
        InputBox(const std::string &title, Size wsize, const Conf &conf)
            : TextBox(title, wsize, conf) {
              };

        ~InputBox() {
        };
    };
}
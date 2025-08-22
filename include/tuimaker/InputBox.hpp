#pragma once

#include <chrono>
#include <tuimaker/TextBox.hpp>
#include <tuimaker/TermUtils.hpp>

namespace tmk
{
    /**
     * @class InputBox
     * @brief TextBox wrapper that enables user input through WindowManager
     *
     */
    class InputBox : public TextBox
    {
    public:
        /**
         * @brief Construct a new Input Box object
         *
         * @param title Window title
         * @param wsize Window size
         * @param conf Window configuration
         */
        InputBox(const std::string &title, Size wsize, const Conf &conf)
            : TextBox(title, wsize, conf) {
              };

        /**
         * @brief Destroy the Input Box object
         *
         */
        ~InputBox() {
        };
    };
}
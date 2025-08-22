#pragma once

#include <functional>

#include <tuimaker/Window.hpp>

namespace tmk
{
    /**
     * @class Button
     * @brief Action button class
     *
     */
    class Button : public Window
    {
    public:
        /**
         * @brief Construct a new Button object given its action's function
         *
         * @param title Window title
         * @param wsize Window size
         * @param conf Window configuration
         * @param key Button key
         * @param f Button function
         */
        Button(const std::string &title, Size wsize, const Conf &conf, const char key, std::function<void(void)> f)
            : key_(key),
              Window(title, wsize, conf)
        {
            func_ = f;
        }

        /**
         * @brief Destroy the Button object
         *
         */
        ~Button()
        {
        }

        /**
         * @brief Get the key of button object
         *
         * @return char&
         */
        auto get_key(void) -> char &
        {
            return key_;
        }

        /**
         * @brief Operator() overload to call function from object
         *
         */
        auto operator()(void) const -> void
        {
            std::lock_guard<std::mutex> lock(mtx_);

            func_();
        }

    private:
        char key_;
        std::function<void(void)> func_;
    };
}
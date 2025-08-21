#pragma once

#include <functional>

#include <tuimaker/Window.hpp>

namespace tmk
{
    class Button : public Window
    {
    public:
        Button(Size wsize, const Conf &conf, const char key, std::function<void(void)> f)
            : key_(key),
              Window(wsize, conf)
        {
            func_ = f;
        }

        ~Button()
        {
        }

        char &get_key(void)
        {
            return key_;
        }

        void operator()(void) const
        {
            std::lock_guard<std::mutex> lock(mtx_);

            func_();
        }

    private:
        char key_;
        std::function<void(void)> func_;
    };
}
#include <iostream>
#include <format>
#include <chrono>
#include <thread>
#include <tuple>

#include <tuimaker/TChar.hpp>
#include <tuimaker/TermUtils.hpp>
#include <tuimaker/WindowManager.hpp>
#include <tuimaker/TextBox.hpp>
#include <tuimaker/LoadingBar.hpp>
#include <tuimaker/InputBox.hpp>

int main(void)
{
    auto wm = tmk::WindowManager::get_instance();

    auto root = wm->create_window<tmk::Window>({0, 0, tmk::TermUtils::get_term_width(), tmk::TermUtils::get_term_height()});
    wm->set_root(root->get_id());
    auto w2 = wm->create_window<tmk::TextBox>({10, 5, 60, 20});

    uint64_t perc = 0;

    for (uint64_t i = 0; i < 32; ++i)
    {
        w2->write(std::format(L"String_{}\n", i));
        wm->render();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    for (uint64_t i = 0; i < 10; ++i)
    {
        w2->scroll_up();
        wm->render();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    for (uint64_t i = 0; i < 16; ++i)
    {
        w2->scroll_down();
        wm->render();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

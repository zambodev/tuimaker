#include <iostream>
#include <format>
#include <chrono>
#include <thread>
#include <tuple>

#include <tuimaker/Utils.hpp>
#include <tuimaker/WindowManager.hpp>
#include <tuimaker/TextBox.hpp>
#include <tuimaker/LoadingBar.hpp>
#include <tuimaker/InputBox.hpp>

int main(void)
{
    tmk::Utils::enable_buff_input();
    tmk::WindowManager *wm = tmk::WindowManager::get_instance();

    auto root = wm->create_window<tmk::Window>({0, 0, tmk::Utils::get_term_width(), tmk::Utils::get_term_height()}, 0);
    auto w2 = wm->create_window<tmk::TextBox>({10, 5, 60, 20}, root->get_id());
    auto w3 = wm->create_window<tmk::LoadingBar>({10, 25, 32, 5}, root->get_id());
    auto w4 = wm->create_window<tmk::InputBox>({70, 5, 32, 5}, root->get_id());

    // for (uint64_t i = 0; i < 10; ++i)
    // {
    //     std::string s = "";
    //     for (uint64_t x = 0; x < i + 1; ++x)
    //     {
    //         s += std::to_string(i);
    //     }
    //     s += "\n";
    //     w4->write(s);
    //     wm->render(root->get_id());
    //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // }

    while (true)
    {
        w4->read();
        // w2->write(std::format("ciao {}\n", ++i));
        wm->render(root->get_id());
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
    }

    tmk::Utils::disable_buff_input();
}

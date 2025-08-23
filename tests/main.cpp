#include <iostream>
#include <format>
#include <chrono>
#include <thread>
#include <tuple>

#include <tuimaker/WindowManager.hpp>
#include <tuimaker/TChar.hpp>
#include <tuimaker/TermUtils.hpp>
#include <tuimaker/TextBox.hpp>
#include <tuimaker/LoadingBar.hpp>
#include <tuimaker/InputBox.hpp>
#include <tuimaker/Button.hpp>

int main(void)
{
    auto wm = tmk::WindowManager::get_instance();
    auto default_conf = tmk::Window::Conf{true, tmk::TChar::TC_DEFAULT, tmk::TChar::TC_DEFAULT, tmk::TChar::BGC_DEFAULT};

    auto [term_w, term_h] = tmk::TermUtils::get_term_size();
    auto root = wm->create_window<tmk::Window>("root", {0, 0, term_w, term_h}, default_conf);
    wm->set_root(root->get_id());
    auto w2 = wm->create_window<tmk::TextBox>("", {10, 5, 60, 20}, {true, tmk::TChar::TC_BLACK, tmk::TChar::TC_WHITE, tmk::TChar::BGC_YELLOW});
    auto w3 = wm->create_window<tmk::LoadingBar>("Something", {10, 25, 32, 5}, default_conf);
    auto w4 = wm->create_window<tmk::InputBox>("wtf", {60, 5, 32, 16}, default_conf);
    auto b1 = wm->create_window<tmk::Button>("Btn1",
                                             tmk::Window::Size{120, 10, 10, 2},
                                             default_conf,
                                             'a',
                                             [&wm, &w2]() -> void
                                             {
                                                 wm->set_on_top(w2->get_id());
                                             });

    wm->select_window(w4->get_id());

    uint64_t counter = 0;
    uint64_t perc = 0;

    while (true)
    {
        // if (((++counter) % 10) == 0)
        //      w3->set(++perc);
        wm->input();
        wm->render(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
    }
}

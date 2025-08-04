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
    auto w3 = wm->create_window<tmk::LoadingBar>({10, 25, 32, 5});
    auto w4 = wm->create_window<tmk::InputBox>({50, 5, 32, 5});

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

    w2->set_bg_color(tmk::TChar::BGC_YELLOW);
    wm->select_window(w4->get_id());

    wm->set_on_top(w2->get_id());
    wm->set_on_top(w4->get_id());

    uint64_t counter = 0;
    uint64_t perc = 0;

    // while (true)
    {
        // if (((++counter) % 10) == 0)
        //     w3->set(++perc);
        // w2->write(std::format("ciao {}\n", ++i));
        wm->render();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
    }
}

#include <iostream>
#include <chrono>
#include <thread>
#include <tuple>

#include <tuimaker/Utils.hpp>
#include <tuimaker/WindowManager.hpp>
#include <tuimaker/TextBox.hpp>

int main(void)
{
    tmk::WindowManager *wm = tmk::WindowManager::get_instance();

    auto rootWindow = wm->create_window<tmk::Window>({0, 0, tmk::Utils::get_term_width(), tmk::Utils::get_term_height()}, 0);
    auto w2 = wm->create_window<tmk::TextBox>({10, 5, 60, 20}, rootWindow->get_id());

    w2->write("ciao\nciao2");
    wm->render(rootWindow->get_id());
}
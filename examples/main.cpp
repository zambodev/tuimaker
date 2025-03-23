#include <iostream>
#include <chrono>
#include <thread>
#include <tuple>

#include <Utils.h>
#include <WindowManager.h>


int main(void)
{
    tmk::WindowManager* wm = tmk::WindowManager::get_instance();

    auto rootWindow = wm->add_window({0, 0, tmk::Utils::get_term_width(), tmk::Utils::get_term_height()}, nullptr);
    auto w2 = wm->add_window({10, 5, 60, 20}, rootWindow);

    w2->write("Ciao come va io tutto bene. Ottimo buona a sapersi dai, spero vada tutto bene. Benissimissimo spero di poter mangiare il cibo.");
    
    wm->render();
}

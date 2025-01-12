#include <iostream>
#include <chrono>
#include <thread>
#include <tuple>

#include <Utils.hpp>
#include <WindowManager.hpp>


int main(void)
{
    tmk::WindowManager* wm = tmk::WindowManager::GetInstance();

    auto rootWindow = wm->AddWindow({0, 0, tmk::Utils::GetTerminalWidth(), tmk::Utils::GetTerminalHeight()}, nullptr);
    auto w2 = wm->AddWindow({10, 5, 60, 20}, rootWindow);

    w2->Write(51, 4, "123 567891 123456789 123456789", 9);
    wm->Render();
}

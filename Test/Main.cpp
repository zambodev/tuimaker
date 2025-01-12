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
    auto w2 = wm->AddWindow({10, 0, 60, tmk::Utils::GetTerminalHeight()}, rootWindow);
    auto w3 = wm->AddWindow({69, 10, tmk::Utils::GetTerminalWidth() - 69, 10}, rootWindow);

    wm->Render();
}

#include <iostream>
#include <chrono>
#include <thread>
#include <tuple>

#include <Utils.hpp>
#include <WindowManager.hpp>

int main(void)
{
    tmk::WindowManager* wm = tmk::WindowManager::GetInstance();

    unsigned int w1Id = wm->AddWindow(new tmk::Window(0, 0, tmk::Utils::GetTerminalWidth(), tmk::Utils::GetTerminalHeight(), 0, 0));

    unsigned int w2Id = wm->AddWindow(new tmk::Window(10, 0, 60, tmk::Utils::GetTerminalHeight(), 0, w1Id));

    unsigned int w3Id = wm->AddWindow(new tmk::Window(69, 10, tmk::Utils::GetTerminalWidth() - 69, 10, 0, w1Id));

    wm->Render();
}

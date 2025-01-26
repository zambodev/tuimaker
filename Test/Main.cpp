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

    w2->Write(1, 1, "Nevertheless, in this mansion of gloom I now proposed to myself a sojourn of some weeks. Its proprietor, Roderick Usher, had been one of my boon companions in boyhood; but many years had elapsed since our last meeting. A letter, however, had lately reached me in a distant part of the country — a letter from him — which, in its wildly importunate nature, had admitted of no other than a personal reply. ");
    wm->Render();
}

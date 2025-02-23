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

    //w2->Write("Ciao come va io tutto bene. Ottimo buona a sapersi dai, spero vada tutto bene. Benissimissimo spero di poter mangiare il cibo.");
    
    wm->Render();

    tmk::Utils::EnableBuffInput();
    char c;
    while((c = std::getchar()))
    {
        w2->WriteChar(c);
        wm->Render();
    }

    tmk::Utils::DisableBuffInput();    
}

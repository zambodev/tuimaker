#include <iostream>
#include <chrono>
#include <thread>
#include <tuple>

#include <WindowManager.hpp>
#include <Utils.hpp>


int main(void)
{
    Window window(0, 0, Utils::GetTerminalWidth(),
                  Utils::GetTerminalHeight(), 0x00, NULL);

    Window child(10, 10, Utils::GetTerminalWidth() / 2, Utils::GetTerminalHeight() / 2, 0x00, &window);
    
    WindowManager::getInstance()->render();

    std::wcout << L"\x1b[0;0H" << std::flush;

    while (1);
}

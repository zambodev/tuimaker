#include <iostream>
#include <chrono>
#include <thread>
#include <tuple>

#include "../src/windowManager.hpp"
#include "../src/window.hpp"
#include "../src/utils.hpp"


int main(void)
{
    Window window(0, 0, UTILS::getTerminalWidth(),
                  UTILS::getTerminalHeight(), 0x00, NULL);

    Window child(10, 10, UTILS::getTerminalWidth() / 2, UTILS::getTerminalHeight() / 2, 0x00, &window);
    
    WindowManager::getInstance()->render();

    std::wcout << L"\x1b[0;0H" << std::flush;

    while (1);
}

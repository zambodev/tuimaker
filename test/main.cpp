#include <iostream>
#include <chrono>
#include <thread>
#include <tuple>

#include "../src/windowManager.hpp"
#include "../src/window.hpp"
#include "../src/utils.hpp"


int main(void)
{
    Window window(0, 10, UTILS::getTerminalWidth(),
    UTILS::getTerminalHeight() - 10, 0x00, NULL);

    Window child(10, 10, 120, 20, 0x00, &window);
    
    window.draw();
    child.draw();

    WindowManager::getInstance()->render();

    std::wcout << L"\x1b[0;0H" << std::endl; 
}

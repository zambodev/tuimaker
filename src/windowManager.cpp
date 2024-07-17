#include <list>
#include "windowManager.hpp"
#include "utils.hpp"


void WindowManager::addWindow(Window& window)
{
    Window *tmp = &window;
    std::wcout << "DEPTH: " << UTILS::getWindowDepth(tmp) << "\n";
}

void WindowManager::removeWindow(Window& window)
{

}

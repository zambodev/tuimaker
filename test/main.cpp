#include <iostream>
#include "../src/windowManager.hpp"
#include "../src/window.hpp"
#include "../src/utils.hpp"

#include <tuple>


int main(void)
{
    auto size = UTILS::getTerminalSize();
    Window window(0, 10, std::move(std::get<0>(size)),
    std::move(std::get<1>(size) - 10), 0x00, NULL);

    WindowManager wm;

    Window child(10, 10, 120, 20, 0x00, &window);
    
    wm.addWindow(window);
    wm.addWindow(child);

    window.draw();
    child.draw();

    wm.render();
}

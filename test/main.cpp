#include <iostream>
#include "../src/windowManager.hpp"
#include "../src/window.hpp"
#include "../src/utils.hpp"

#include <tuple>


int main(void)
{
    Window window(0, 10, UTILS::getTerminalWidth(),
    UTILS::getTerminalHeight() - 10, 0x00, NULL);

    WindowManager wm;

    Window child(10, 10, 120, 20, 0x00, &window);
    
    wm.addWindow(window);
    wm.addWindow(child);

    window.draw();
    child.draw();

    wm.render();
}

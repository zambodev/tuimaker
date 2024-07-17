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

    Window child(20, 10, 40, 20, 0x00, &window);
    Window child1(20, 10, 40, 20, 0x00, &child);
    Window child2(20, 10, 40, 20, 0x00, &child1);
    Window child3(20, 10, 40, 20, 0x00, &child2);
    
    WindowManager wm;

    wm.addWindow(child3);

    //window.draw();
    //child.draw();
}

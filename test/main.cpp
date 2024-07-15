#include <iostream>
#include "../src/window.hpp"
#include "../src/utils.hpp"

#include <tuple>


int main(void)
{
    auto size = UTILS::getTerminalSize();
    Window window(0, 10, std::move(std::get<0>(size)), std::move(std::get<1>(size) - 10));

    Window child(20, 10, 20, 20, 0, window);
    window.show();
    child.show();
}

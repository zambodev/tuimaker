#include <iostream>
#include "../src/box.hpp"
#include "../src/utils.hpp"

#include <tuple>


int main(void)
{
    auto size = UTILS::getTerminalSize();
    Box window(0, 10, std::move(std::get<0>(size)), std::move(std::get<1>(size) - 10), 0x00);

    Box child(20, 10, 40, 20, 0x00);
    
    window.draw();
    child.draw();
}

#include <iostream>
#include "../src/window.hpp"
#include "../src/utils.hpp"

#include <tuple>


int main(void)
{
    auto size = UTILS::getTerminalSize();
    Window window(0, 0, std::get<0>(size), std::get<1>(size));
}

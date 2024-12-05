#include <iostream>
#include <chrono>
#include <thread>
#include <tuple>

#include <Utils.hpp>
#include <Window.hpp>

int main(void)
{
    std::cout <<  tmk::Utils::GetTerminalHeight();
    return 0;

}

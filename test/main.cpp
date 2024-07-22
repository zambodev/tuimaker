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

    WindowManager wm;

    Window child(10, 10, 120, 20, 0x00, &window);
    
    wm.addWindow(window);
    wm.addWindow(child);

    window.draw();
    child.draw();

    double avg = 0.0;
    for(int i = 0; i < 50; ++i)
    {
        const auto start{std::chrono::steady_clock::now()};

        wm.render();

        const auto end{std::chrono::steady_clock::now()};

        const std::chrono::duration<double> elapsed_seconds{end - start};
        avg += elapsed_seconds.count();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }


    std::wcout << L"\x1b[0;0H" << avg / 100 << std::endl;     // 0.00133 - 0.00156
}

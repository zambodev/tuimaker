#include <iostream>
#include <thread>
#include <array>
#include <tui.hpp>

void ciao(void)
{
	wprintf(L"\x1b[30;30HCIAO");
	fflush(stdout);
}

int main()
{
	Tui *tui;
	tui = tui->get_instance("Test App");

	int cols, rows;
	std::array<int, 2> size = tui->get_size();


	tui->create_box("box1", size[0] - 30, size[1] - 20, size[0] - 1, size[1] - 1, "Notifications", "User123: You're welcome\nUser365: Nvm");
	tui->refresh();

	sleep(1);

	tui->delete_box("box1");
	tui->refresh();

	while(1);
}

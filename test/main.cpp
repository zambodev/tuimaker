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


	tui->create_box("box1", size[0] - 30, size[1] - 20, size[0] - 1, size[1] - 1, "Files", "test.exepokemon.datbrowser.exe");
	tui->refresh();

	tui->input_selec("sel1");

	while(1);
}

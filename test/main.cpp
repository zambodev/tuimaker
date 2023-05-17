#include <iostream>
#include <thread>
#include <tui.hpp>

void ciao(void)
{
	wprintf(L"\x1b[30;30HCIAO");
	fflush(stdout);
}

int main()
{
	Tui *tui;
	tui = tui->get_instance("Prova");

	tui->create_box("box1", 10, 10, 40, 20, "Messagges", "Nothing to see");
	tui->create_selec("sel1", 50, 10, {"Say 'Ciao'"}, {ciao});
	tui->refresh();

	tui->input_selec("sel1");

	while(1);
}

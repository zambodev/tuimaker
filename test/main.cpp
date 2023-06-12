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

	tui->create_box("notification", tui->get_size()[0] - 30, tui->get_size()[1] - 20, tui->get_size()[0], tui->get_size()[1], "Notifications");

	tui->create_selec("options", 2, tui->get_size()[1] - 1, 0, {"Notifcation", "GINO"}, {[tui](){tui->draw_box("notification"); tui->write_box("notification", {"Nothing to see"}); tui->refresh();}, ciao});
	tui->draw_selec("options");
	tui->refresh();


	tui->input_selec("options");
	
	while(1);
}

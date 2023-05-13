#include <iostream>
#include <tui.hpp>

void ciao(void)
{
	wprintf(L"\x1b[10;10HCIAO");
	fflush(stdout);
}

int main()
{
	Window window("TUILIB testing");
	window.get_box("main")->write("Prova testo");
	window.create_box("Inbox", 10, 10, 40, 20, "Inbox", "Nessun messaggio");
	window.refresh();

	window.delete_box("Inbox");
	window.refresh();

	window.create_selec("prova", 5, 5, {"provetta", "prina", "provaccia"}, {&ciao, &ciao, &ciao});
	window.refresh();

	window.get_selec("prova")->clear();
	window.refresh();

	while(1);
}

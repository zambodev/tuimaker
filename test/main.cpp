#include <iostream>
#include <clocale>
#include <tui.hpp>


int main()
{
	setlocale(LC_ALL, "");

	Window window("TUILIB testing");
	window.get_box("main")->write("Prova testo");
	window.create_box("Inbox", 10, 10, 40, 20, "Inbox", "Nessun messaggio");
	window.refresh();


	while(1);
}

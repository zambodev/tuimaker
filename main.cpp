#include <iostream>
#include <clocale>
#include "Window.hpp"


int main()
{
	setlocale(LC_ALL, "");

	Window window("TUILIB testing");

	window.refresh();

	window.get_box("main")->write("Prova testo");
	window.set_box("Inbox", 10, 10, 40, 30, "Inbox", "Nessun messagio");
	window.refresh();

	sleep(4);

	window.get_box("Inbox")->~Box();
	window.refresh();

	while(1);
}

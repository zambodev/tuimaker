#include <iostream>
#include <clocale>
#include "Window.hpp"


int main()
{
	setlocale(LC_ALL, "");

	Window window("TUILIB testing");

	window.refresh();

	window.boxes["main"]->write("Prova testo");
	window.refresh();

	sleep(4);

	window.boxes["main"]->clear_text();
	window.refresh();

	while(1);
}

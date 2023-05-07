#include <iostream>
#include <clocale>
#include "Window.hpp"


int main()
{
	setlocale(LC_ALL, "");

	Window window("TUILIB testing");

	window.refresh();


	while(1);
}

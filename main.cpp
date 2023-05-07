#include <iostream>
#include <clocale>
#include "draw.hpp"


int main()
{
	setlocale(LC_ALL, "");

	Window draw("TUILIB testing");

	draw.box(1, 1, 40, 17, 1);

	draw.refresh();


	while(1);
}

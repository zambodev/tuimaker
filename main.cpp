#include <iostream>
#include <clocale>
#include "draw.hpp"


int main()
{
	setlocale(LC_ALL, "");

	Window draw("TUILIB testing");

	draw.refresh();

	while(1);

}

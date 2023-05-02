#include <iostream>
#include <clocale>
#include "draw.hpp"


int main()
{
	setlocale(LC_ALL, "");

	Draw draw;

	draw.hline(5, 5, 10, 5);

	draw.refresh();

	while(1);

}

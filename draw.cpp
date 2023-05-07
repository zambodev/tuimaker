#include <cstring>
#include "draw.hpp"


Window::Window(std::string title)
{	
#ifdef __linux__
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	cols = w.ws_col;
	rows = w.ws_row;
#elif _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#endif

	buffer = (wchar_t *)malloc(sizeof(wchar_t) * cols * rows);

	// Fill with blank
	for(int i = 0; i < cols * rows; ++i)
		buffer[i] = u'\u0020';

	box(0, 0, cols-1, rows-1, 0, title);

}

void Window::refresh(void)
{
	wprintf(L"\x1b[0;0H");

	for(int i = 0; i < cols * rows; ++i)
	{
		wprintf(L"%lc", buffer[i]);
	}

	if(input_pos[0] != -1 && input_pos[1] != -1)
		wprintf(L"\x1b[%d;%dH", input_pos[1], input_pos[0]);

	fflush(stdout);
}

void Window::box(int x1, int y1, int x2, int y2, int type, std::string title)
{
	// Top corner
	buffer[cols * y1 + x1] = tlcrn;
	buffer[cols * y1 + x1 + (x2 - x1)] = trcrn;
	// Bottom corner
	buffer[cols * y2 + x1] = blcrn;
	buffer[cols * y2 + x1 + (x2 - x1)] = brcrn;
	// Fill top and bottom lines
	for(int i = x1 + 1; i < x2; ++i)
	{
		buffer[cols * y1 + i] = hbar;
		buffer[cols * y2 + i] = hbar;	
	}
	// Left and right lines
	for(int i = y1 + 1; i < y2; ++i)
	{
		buffer[cols * i + x1] = vbar;
		buffer[cols * i + x1 + (x2 - x1)] = vbar;
	}
	// Print title
	for(int i = 0; i < title.length(); ++i)
		buffer[cols * (y1 + 1) + (x2 - x1) / 2 - title.length() / 2 + i] = title[i];

	if(type)
	{
		input_pos[0] = x1 + 3;
		input_pos[1] = y1 + (y2 - (y1 - 1)) / 2 + 1;
	}
}

void Window::hrzline(int x1, int y1, int x2, int y2)
{
	for(int i = x1; i < x2; ++i)
		buffer[y1 * rows + i] = hbar;
}


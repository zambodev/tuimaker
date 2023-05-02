#include <cstring>
#include "draw.hpp"


Draw::Draw()
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

	for(int i = 0; i < cols * rows; ++i)
		buffer[i] = u'\u0020';
}

void Draw::refresh()
{
	for(int i = 0; i < cols * rows; ++i)
	{
		wprintf(L"%lc", buffer[i]);
	}

	fflush(stdout);
}

void Draw::hline(int x1, int y1, int x2, int y2)
{
	for(int i = x1; i < x2; ++i)
		buffer[y1 * rows + i] = hbar;
}


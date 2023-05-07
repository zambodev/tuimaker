#include <cstring>
#include "tui.hpp"


/* class Box*/

Window::Box::Box(int x1, int y1, int x2, int y2, std::string title, std::string text)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;

	if(title != "") this->title = title;
	draw();

	if(text != "")
	{
		this->text = text;
		write(text);
	}
}

Window::Box::~Box()
{
	clear();
}

void Window::Box::draw(void)
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
		buffer[cols * (y1 + 1) + x1 + (x2 - x1) / 2 - title.length() / 2 + i] = title[i];
}

void Window::Box::write(std::string text)
{
	int i = y1 + 2, idx = 0
	;
	if(!this->title.empty()) ++i;

	for(i; i < y2; ++i)
		for(int j = x1 + 2; j < x2; ++j)
			if(text[idx])
				Window::buffer[Window::cols * i + j] = text[idx++];
			else break;
}

void Window::Box::clear_text(void)
{
	int i = y1 + 1;
	if(!this->title.empty()) ++i;

	for(i; i < y2; ++i)
		for(int j = x1 + 1; j < x2; ++j)
			Window::buffer[Window::cols * i + j] = L' ';
}

void Window::Box::clear(void)
{
	for(int i = y1; i <= y2; ++i)
		for(int j = x1; j <= x2; ++j)
			Window::buffer[Window::cols * i + j] = L' ';
}


/* Class Window */

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

	boxes.insert({"main", new Box(0, 0, cols-1, rows-1, title)});
}

void Window::refresh(void)
{
	wprintf(L"\x1b[0;0H");

	for(int i = 0; i < cols * rows; ++i)
		wprintf(L"%lc", buffer[i]);

	fflush(stdout);
}

void Window::create_box(std::string id, int x1, int y1, int x2, int y2, std::string title, std::string text)
{
	boxes.try_emplace(id, new Box(x1, y1, x2, y2, title, text));
}

void Window::delete_box(std::string id)
{
	boxes[id]->~Box();
	boxes.erase("id");
}

Window::Box * Window::get_box(std::string id)
{
	return boxes[id];
}

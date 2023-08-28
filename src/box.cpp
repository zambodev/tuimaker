#include "../include/window.hpp"


Window::Box::Box(int x1, int y1, int x2, int y2, std::string title)
{
#ifdef DEBUG
	std::cerr << "box(): creating box object\n";
#endif

	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;

	copy = (wchar_t *)malloc((x2 - x1 + 1) * (y2 - y1 + 1) * sizeof(wchar_t));
	int idx = 0;

	for(int i = y1; i <= y2; ++i)
		for(int j = x1; j <= x2; ++j)
			copy[idx++] = buffer[cols * i + j];

	if(title != "") this->title = title;
}

Window::Box::~Box()
{
	delete copy;

	clear();
}

void Window::Box::draw(void)
{
#ifdef DEBUG
	std::cerr << "draw(): drwaing box into the buffer\n";
#endif

	// Top left corner
	if(buffer[cols * y1 + x1] == hbar) 
		buffer[cols * y1 + x1] = tint;
	else if(buffer[cols * y1 + x1] == vbar) 
		buffer[cols * y1 + x1] = lint;
	else
		buffer[cols * y1 + x1] = tlcrn;

	// Top right corner
	if(buffer[cols * y1 + x1 + (x2 - x1)] == hbar) 
		buffer[cols * y1 + x1 + (x2 - x1)] = tint;
	else if(buffer[cols * y1 + x1 + (x2 - x1)] == vbar) 
		buffer[cols * y1 + x1 + (x2 - x1)] = rint;
	else
		buffer[cols * y1 + x1 + (x2 - x1)] = trcrn;

	// Bottom left corner
	if(buffer[cols * y2 + x1] == hbar) 
		buffer[cols * y2 + x1] = bint;
	else if(buffer[cols * y2 + x1] == vbar) 
		buffer[cols * y2 + x1] = lint;
	else
		buffer[cols * y2 + x1] = blcrn;

	// Bottom right corner
	if(buffer[cols * y2 + x1 + (x2 - x1)] == hbar) 
		buffer[cols * y2 + x1 + (x2 - x1)] = bint;
	else if(buffer[cols * y2 + x1 + (x2 - x1)] == vbar) 
		buffer[cols * y2 + x1 + (x2 - x1)] = rint;
	else
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

#ifdef DEBUG
	std::cerr << "draw(): writing box' title into the buffer\n";
#endif

	// Print title
	for(int i = 0; i < title.length(); ++i)
		buffer[cols * (y1 + 1) + x1 + (x2 - x1) / 2 - title.length() / 2 + i] = title[i];
}

void Window::Box::move(int x1, int y1, int x2, int y2)
{
	clear();

#ifdef DEBUG
	std::cerr << "move(): changing box coordinates\n";
#endif

	if(x2 == -1 && y2 == -1)
	{
		this->x2 += x1 - this->x1;
		this->y2 += y1 - this->y1;
	}
	else
	{
		this->x2 = x2;
		this->y2 = y2;
	}

	this->x1 = x1;
	this->y1 = y1;
}

void Window::Box::write(std::vector<std::string> text)
{
#ifdef DEBUG
	std::cerr << "write(): writing box' text into the buffer\n";
#endif

	int i = y1 + 2;
	if(!this->title.empty()) ++i;

	for(std::string str : text)
	{
		for(int j = x1 + 2, idx = 0; j < x2 && idx < str.size(); ++j, ++idx)
			buffer[cols * i + j] = str[idx];

		++i;
	}
}

void Window::Box::clear_text(void)
{
#ifdef DEBUG
	std::cerr << "clear_text(): removing box' text from the buffer\n";
#endif

	int i = y1 + 1;
	if(!this->title.empty()) ++i;

	for(i; i < y2; ++i)
		for(int j = x1 + 1; j < x2; ++j)
			buffer[cols * i + j] = L' ';
}

void Window::Box::clear(void)
{
#ifdef DEBUG
	std::cerr << "clear(): removing box from the buffer\n";
#endif

	int idx = 0;

	for(int i = y1; i <= y2; ++i)
		for(int j = x1; j <= x2; ++j)
			buffer[cols * i + j] = copy[idx++];
}
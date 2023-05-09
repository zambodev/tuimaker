#include "../include/Window.hpp"


Window::Selectable::Selectable(int x, int y, std::vector<std::string> options, std::vector<std::function<void()>> funcs) 
{	
	this->x = x;
	this->y = y;

	for(int i = 0; i < options.size(); ++i)
	{
		this->options.try_emplace(i, new Option(options.at(i), funcs.at(i)));
	}

	draw();
}

Window::Selectable::~Selectable()
{
	clear();
}

void Window::Selectable::draw(void)
{
	int xt = x;
	int yt = y;
	for(int i = 0; i < options.size(); ++i)
	{
		buffer[cols * yt + xt] = 0x030 + i + 1;
		buffer[cols * yt + xt + 1] = ' ';

		std::string &tmp = options.at(i)->name;
		for(int j = 0; j < tmp.length(); ++j)
		{
			buffer[cols * yt + xt + j + 2] = tmp[j];
		}
		++yt;
	}
}

void Window::Selectable::clear(void)
{
	size_t maxlen = 0;
	for(int i = 0; i < options.size(); ++i)
	{
		size_t len = options.at(i)->name.length();
		if(len > maxlen)
			maxlen = len;
	}

	for(int i = x; i <= x + options.size(); ++i)
		for(int j = y; j <= y + maxlen + 2; ++j)
			buffer[cols * i + j] = L' ';
}

#include "../include/window.hpp"


Window::Selectable::Selectable(int x, int y, bool is_row, std::vector<std::string> options, std::vector<std::function<void()>> funcs) 
{	
#ifdef DEBUG
	std::cerr << "Selectable(): creating selectable object\n";
#endif
	this->x = x;
	this->y = y;
	this->is_row = is_row;

#ifdef DEBUG
	std::cerr << "Selectable(): emplacing options\n";
#endif

	for(int i = 0; i < options.size(); ++i)
	{
		this->options.try_emplace(i, new Option(options.at(i), funcs.at(i)));
	}
}

Window::Selectable::~Selectable()
{
#ifdef DEBUG
std::cerr << "~Selectable()\n";
#endif

	clear();

	/* Clear options */
	for(const auto& option : options)
	{
		delete option.second;
	}
	options.clear();
}

void Window::Selectable::draw(void)
{
#ifdef DEBUG
	std::cerr << "draw(): drawing selectable to buffer\n";
#endif

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

		if(!is_row)
			++yt;
		else 
			xt += tmp.length() + 4;
	}
}

void Window::Selectable::clear(void)
{
#ifdef DEBUG
	std::cerr << "clear(): removing selectable from buffer\n";
#endif

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

void Window::Selectable::select(int id)
{
#ifdef DEBUG
	std::cerr << "select(): selecting option from selectable\n";
#endif
	try
	{
		options.at(id - 1)->func();
	}
	catch(std::out_of_range)
	{
#ifdef DEBUG
	std::cerr << "select(): invalid option\n";
#endif

		return;
	}
}

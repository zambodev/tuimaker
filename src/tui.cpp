#include "../include/tui.hpp"


Tui * Tui::instance = nullptr;

Tui::Tui(std::string title)
{	
	window = new Window(title);
	queue_thd = new std::thread(&Tui::check_queue, this);
}

Tui::~Tui()
{
	queue_thd->join();
	delete [] window;
	window = nullptr;
}

void Tui::check_queue()
{
	while(1)
	{
		if(queue.empty()) continue;

		std::wcout << "Found a func!\n";
		do
		{
			auto f = queue.front();
			queue.pop();

			f();
		}
		while(!queue.empty());
	}
}

Tui * Tui::get_instance(std::string title)
{	
	if(!instance)
		instance = new Tui(title);

	return instance;
}

void Tui::refresh(void)
{
	std::function<void()> f = [this]()
		{
			window->refresh();
		};

	queue.push(f);
}

std::array<int, 2> Tui::get_size(void)
{	
	std::array<int, 2> size = window->get_size();

	return size;
}


void Tui::create_box(std::string id, int x1, int y1, int x2, int y2, std::string title)
{
	thd = new std::jthread([this, id, x1, y1, x2, y2, title]()
	{
		window->create_box(id, x1, y1, x2, y2, title);
	});
}

void Tui::delete_box(std::string id)
{
	thd = new std::jthread([this, id]()
	{
		window->delete_box(id);
	});
}

void Tui::draw_box(std::string id)
{
	thd = new std::jthread([this, id]()
	{
		window->get_box(id)->draw();
	});
}

void Tui::move_box(std::string id, int x1, int y1, int x2, int y2)
{
	thd = new std::jthread([this, id, x1, y1, x2, y2]()
	{
		window->get_box(id)->move(x1, y1, x2, y2);
	});
}

void Tui::write_box(std::string id, std::vector<std::string> text)
{
	thd = new std::jthread([this, id, text]()
	{
		window->get_box(id)->write(text);
	});
}

void Tui::clear_box(std::string id)
{
	thd = new std::jthread([this, id]()
	{		
		window->get_box(id)->clear();
	});
}

void Tui::clear_text_box(std::string id)
{
	thd = new std::jthread([this, id]()
	{
		window->get_box(id)->clear_text();
	});
}

void Tui::create_selec(std::string id, int x, int y, int dir, std::vector<std::string> options, std::vector<std::function<void(void)>> funcs)
{
	thd = new std::jthread([this, id, x, y, dir, options, funcs]()
	{
		window->create_selec(id, x, y,dir, options, funcs);
	});
}

void Tui::draw_selec(std::string id)
{
	thd = new std::jthread([this, id]()
	{
		window->get_selec(id)->draw();
	});
}

void Tui::delete_selec(std::string id)
{
	thd = new std::jthread([this, id]()
	{
		window->delete_selec(id);
	});
}

void Tui::input_selec(std::string id)
{
	thd = new std::jthread([this, id]()
	{
		unsigned char value;
#ifdef __linux__
		struct termios old_tio, new_tio;

		tcgetattr(STDIN_FILENO, &old_tio);
		new_tio = old_tio;
		new_tio.c_lflag &=(~ICANON & ~ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

		do
		{
			value = getchar();
		}
		while(value < '1' || value > '9');

		tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
#elif _WIN32
		do
		{
			value = getch();
		}
		while(value < '1' || value > '9');
#endif
		window->get_selec(id)->select(value - '0');
	});
}

void Tui::clear_selec(std::string id)
{
	thd = new std::jthread([this, id]()
	{
		window->get_selec(id)->clear();
	});
}

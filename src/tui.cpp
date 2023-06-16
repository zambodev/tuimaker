#include "../include/tui.hpp"


Tui * Tui::instance = nullptr;

Tui::Tui(std::string title)
{	
	this->title = title;
	is_input = true;
	is_running = true;
	input_lock = false;
	queue_lock = false;
	window = new Window(title);
	queue_thd = new std::thread(&Tui::check_queue, this);
	input_thd = new std::thread(&Tui::check_input, this);
}

Tui::~Tui()
{
	queue_thd->join();
	input_thd->join();
	delete [] window;
	window = nullptr;
}

void Tui::check_queue()
{
	while(1)
	{
		if(queue.empty()) continue;

		do
		{
			auto f = queue.front();
			queue.pop();
			
			f();
		}
		while(!queue.empty());
	}
}

void Tui::check_input()
{
	unsigned char value;

#ifdef __linux__
	struct termios old_tio, new_tio;

	while(is_running)
	{	
		input_lock.wait(true);
		input_lock = true;
		
		bool mode = this->is_input;
		
		input_lock = false;
		input_lock.notify_all();

		if(!mode)	// Command mode
		{
			tcgetattr(STDIN_FILENO, &old_tio);
			new_tio = old_tio;
			new_tio.c_lflag &=(~ICANON & ~ECHO);
			tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

			do
			{
				value = getchar();
				std::wcout << value << std::fflush;
			}
			while(value < '0' || value > '9');

			tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

			std::function<void()> f = [this, value]()
			{
				window->get_selec(title)->select(value - 48);
			};

			queue.push(f);
		}
		else 			// Text mode
		{
			//std::wcout << "input mode";
			continue;
		}
	}
#elif _WIN32
	do
	{
		value = getch();
	}
	while(value == EOF);
#endif
}

void Tui::input_mode(std::string mode)
{
	input_lock.wait(true);
	input_lock = true;

	if(mode == "input")
		this->is_input = true;
	else if(mode == "command")
		this->is_input = false;

	input_lock = false;
	input_lock.notify_all();

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
	std::function<void()> f = [this, id, x1, y1, x2, y2, title]()
	{
		window->create_box(id, x1, y1, x2, y2, title);
	};

	queue.push(f);
}

void Tui::delete_box(std::string id)
{
	std::function<void()> f = [this, id]()
	{
		window->delete_box(id);
	};

	queue.push(f);
}

void Tui::draw_box(std::string id)
{
	std::function<void()> f = [this, id]()
	{
		window->get_box(id)->draw();
	};

	queue.push(f);
}

void Tui::move_box(std::string id, int x1, int y1, int x2, int y2)
{
	std::function<void()> f = [this, id, x1, y1, x2, y2]()
	{
		window->get_box(id)->move(x1, y1, x2, y2);
	};

	queue.push(f);
}

void Tui::write_box(std::string id, std::vector<std::string> text)
{
	std::function<void()> f = [this, id, text]()
	{
		window->get_box(id)->write(text);
	};

	queue.push(f);
}

void Tui::clear_box(std::string id)
{
	std::function<void()> f = [this, id]()
	{		
		window->get_box(id)->clear();
	};
	
	queue.push(f);
}

void Tui::clear_text_box(std::string id)
{
	std::function<void()> f = [this, id]()
	{
		window->get_box(id)->clear_text();
	};

	queue.push(f);
}

void Tui::create_selec(std::string id, int x, int y, bool is_row, std::vector<std::string> options, std::vector<std::function<void(void)>> funcs)
{
	std::function<void()> f = [this, id, x, y, is_row, options, funcs]()
	{
		window->create_selec(id, x, y,is_row, options, funcs);
	};

	queue.push(f);
}

void Tui::draw_selec(std::string id)
{
	std::function<void()> f = [this, id]()
	{
		window->get_selec(id)->draw();
	};

	queue.push(f);
}

void Tui::delete_selec(std::string id)
{
	std::function<void()> f = [this, id]()
	{
		window->delete_selec(id);
	};

	queue.push(f);
}

void Tui::clear_selec(std::string id)
{
	std::function<void()> f = [this, id]()
	{
		window->get_selec(id)->clear();
	};

	queue.push(f);
}

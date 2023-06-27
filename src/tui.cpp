#include "../include/tui.hpp"


Tui * Tui::instance = nullptr;

Tui::Tui(std::string title)
{	
	tcgetattr(STDIN_FILENO, &old_tio);
	new_tio = old_tio;
	new_tio.c_lflag &=(~ICANON & ~ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

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
	is_running = false;

	while(!queue_thd->joinable());
	queue_thd->join();
	
	while(!input_thd->joinable());
	input_thd->join();
	
	delete [] window;
	delete [] instance;
	instance = nullptr;
	window = nullptr;

	tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
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
	std::string str = "";

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
#ifdef __linux__
				value = getchar();
#elif _WIN32
				value = getch();
#endif
			}
			while((value < '0' || value > '9') && value != 'i');

			tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

			if(value == 'i')
			{
				input_lock.wait(true);
				input_lock = true;

				this->is_input = true;

				input_lock = false;
				input_lock.notify_all();
			}
			else if(window->get_selec(title) != nullptr)
			{
				std::function<void()> f = [this, value]()
				{
					window->get_selec(title)->select(value - 48);
				};

				queue.push(f);
			}
		}
		else 			// Text mode
		{
			wprintf(L"\x1b[u");

#ifdef __linux__
			value = getchar();
#elif _WIN32	
			value = getch();
#endif
			/* if ESC is pressed exit input mode*/
			if(value == 27)
			{
				wprintf(L"\x1b[s");

				str.append(1, '\n');

				input_lock.wait(true);	
				input_lock = true;
			
				input_queue.push(str);
				str.clear();

				this->is_input = false;

				input_lock = false;
				input_lock.notify_all();				
			}
			/* If BACKSPACE is pressed delete the last char from str*/
			else if(value == 127)
			{
				wprintf(L"\x1b[1D \x1b[1D\x1b[s");
				if(!str.empty())
					str.pop_back();
			}
			/* if ENTER is pressed end the str and ini a new onw*/
			else if(value == 10)
			{
				str.append(1, '\n');
				
				input_lock.wait(true);	
				input_lock = true;
				
				input_queue.push(str);
				
				input_lock = false;
				input_lock.notify_all();
				
				str.clear();
			}
			else
			{
				wprintf(L"\x1b[u%c\x1b[s", value);
				str.append(1, value);
			}
		}
	}
}

std::string Tui::get_input()
{
	if(!input_queue.empty())
	{
		std::string tmp = input_queue.front();
		input_queue.pop();

		return tmp;
	}

	return "";
}


void Tui::input_mode(std::string mode)
{
	input_lock.wait(true);
	input_lock = true;

	if(mode == "input")
	{
		this->is_input = true;
	}
	else if(mode == "command")
	{
		wprintf(L"\x1b[s");
		this->is_input = false;
	}

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

void Tui::box_create(std::string id, int x1, int y1, int x2, int y2, std::string title)
{
	std::function<void()> f = [this, id, x1, y1, x2, y2, title]()
	{
		window->box_create(id, x1, y1, x2, y2, title);
	};

	queue.push(f);
}

void Tui::box_delete(std::string id)
{
	std::function<void()> f = [this, id]()
	{
		window->box_delete(id);
	};

	queue.push(f);
}

void Tui::box_draw(std::string id)
{
	std::function<void()> f = [this, id]()
	{
		window->get_box(id)->draw();
	};

	queue.push(f);
}

void Tui::box_move(std::string id, int x1, int y1, int x2, int y2)
{
	std::function<void()> f = [this, id, x1, y1, x2, y2]()
	{
		window->get_box(id)->move(x1, y1, x2, y2);
	};

	queue.push(f);
}

void Tui::box_write(std::string id, std::vector<std::string> str_arr)
{
	std::function<void()> f = [this, id, str_arr]()
	{
		window->get_box(id)->write(str_arr);
	};

	queue.push(f);
}

void Tui::box_clear(std::string id)
{
	std::function<void()> f = [this, id]()
	{		
		window->get_box(id)->clear();
	};
	
	queue.push(f);
}

void Tui::box_clear_text(std::string id)
{
	std::function<void()> f = [this, id]()
	{
		window->get_box(id)->clear_text();
	};

	queue.push(f);
}

void Tui::selec_create(std::string id, int x, int y, bool is_row, std::vector<std::string> options, std::vector<std::function<void(void)>> funcs)
{
	std::function<void()> f = [this, id, x, y, is_row, options, funcs]()
	{
		window->selec_create(id, x, y,is_row, options, funcs);
	};

	queue.push(f);
}

void Tui::selec_draw(std::string id)
{
	std::function<void()> f = [this, id]()
	{
		window->get_selec(id)->draw();
	};

	queue.push(f);
}

void Tui::selec_input(std::string id)
{
	std::function<void()> f = [this, id]()
	{
		unsigned char value;
#ifdef __linux__
		do
		{
			value = getchar();
		}
		while(value < '1' || value > '9');
#elif _WIN32
		do
		{
			value = getch();
		}
		while(value < '1' || value > '9');
#endif
		window->get_selec(id)->select(value - '0');
	};

	queue.push(f);
}

void Tui::selec_delete(std::string id)
{
	std::function<void()> f = [this, id]()
	{
		window->selec_delete(id);
	};

	queue.push(f);
}

void Tui::selec_clear(std::string id)
{
	std::function<void()> f = [this, id]()
	{
		window->get_selec(id)->clear();
	};

	queue.push(f);
}

#include "../include/tui.hpp"


Tui::Tui(std::string title)
{	
	this->title = title;
	is_input = true;
	is_running = true;
	input_lock = false;
	run_lock = false;
	queue_lock = false;
	window = new Window(title);
	queue_thd = new std::thread(&Tui::check_queue, this);
	input_thd = new std::thread(&Tui::check_input, this);
}

Tui::~Tui()
{
	run_lock.wait(true);
	run_lock = true;
	
	is_running = false;
	
	run_lock = false;
	run_lock.notify_all();

	while(!input_thd->joinable());
	input_thd->join();

	while(!queue_thd->joinable());
	queue_thd->join();

	wprintf(L"CHECKMATE!");
	fflush(stdout);

	delete window;
	window = nullptr;

	wprintf(L"\x1b[0;0H");
}

void Tui::check_queue()
{
	bool run = true;

	while(run)
	{
		run_lock.wait(true);
		run_lock = true;
		
		run = is_running;
		
		run_lock = false;
		run_lock.notify_all();
		if(queue.empty()) continue;

		if(!run) break;

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
	bool run = true;
	fd_set rfds;
	struct timeval tv;
	std::string str = "";
	std::string eof;

	#ifdef __linux__
	eof = "\n";
	#elif _WIN32
	eof = "\r\n";
	#endif

	while(run)
	{	
		run_lock.wait(true);
		run_lock = true;
		
		run = is_running;
		
		run_lock = false;
		run_lock.notify_all();

		if(!run) return;

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

			FD_ZERO(&rfds);
			FD_SET(0, &rfds);

			tv.tv_sec = 0;
			tv.tv_usec = 100;

			if(select(1, &rfds, NULL, NULL, &tv) <= 0)
			{
				tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
				continue;
			}

			#ifdef __linux__
			value = getchar();
			#elif _WIN32
			value = getch();
			#endif

			tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

			if((value < '0' || value > '9') && value != 'i') continue;

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
			wprintf(L"\x1b[%d;%dH", input_y, input_x);
			
			tcgetattr(STDIN_FILENO, &old_tio);
			new_tio = old_tio;
			new_tio.c_lflag &=(~ICANON & ~ECHO);
			tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

			FD_ZERO(&rfds);
			FD_SET(0, &rfds);

			tv.tv_sec = 0;
			tv.tv_usec = 100;

			if(select(1, &rfds, NULL, NULL, &tv) <= 0)
			{
				tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
				continue;
			}

			#ifdef __linux__
			value = getchar();
			#elif _WIN32	
			value = getch();
			#endif

			tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

			switch(value)
			{
				case 27:					// ESC
					str.append(eof);

					input_lock.wait(true);	
					input_lock = true;
				
					input_queue.push(str);
					str.clear();

					this->is_input = false;

					input_lock = false;
					input_lock.notify_all();				
					break;

				case 127:					// BACKSPACE
					wprintf(L"\x1b[1D \x1b[1D\x1b[s");
					--input_x;
					if(!str.empty())
						str.pop_back();
					break;

				case 10:					// ENTER
					if(input_adv) ++input_y;

					str.append(eof);
					
					input_lock.wait(true);	
					input_lock = true;
					
					input_queue.push(str);
					
					input_lock = false;
					input_lock.notify_all();
					
					str.clear();
					break;

				default:
					wprintf(L"%c", value);
					write(input_x, input_y, value);
					str.append(1, value);
					++input_x;
					break;
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

void Tui::input_cords(int x, int y, bool adv)
{
	input_x = x;
	input_y = y;
	input_adv = adv;
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

void Tui::write(int x, int y, char c)
{	
	std::function<void()> f = [this, x, y, c]()
	{
		window->write(x, y, c);
	};

	queue.push(f);
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

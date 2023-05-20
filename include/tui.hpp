#ifndef TUI_HPP
#define TUI_HPP

#include <thread>
#include <atomic>
#include <array>
#ifdef __linux__
#include <termios.h>
#elif _WIN32
#include <conio.h>
#endif
#include "Window.hpp"


class Tui
{
	private:
		std::jthread *thd;
		std::atomic<bool> locked;
		static Tui *instance;
		Window *window;
		Tui(std::string title);
		~Tui();

	public:
		void operator=(const Tui &) = delete;
		Tui(Tui &) = delete;
		static Tui * get_instance(std::string title);

		void refresh(void);
		std::array<int, 2> get_size(void);
		/* Box */
		void create_box(std::string, int, int, int, int, std::string = "", std::string = "");
		void delete_box(std::string);
		void move_box(std::string, int, int, int = -1, int = -1);
		void write_box(std::string, std::string);
		void clear_text_box(std::string);
		/* Selectable */
		void create_selec(std::string, int, int, std::vector<std::string>, std::vector<std::function<void(void)>>);
		void delete_selec(std::string);
		void input_selec(std::string id);
};

#endif
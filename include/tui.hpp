#ifndef TUI_HPP
#define TUI_HPP

#include <thread>
#include <atomic>
#include <array>
#include <queue>
#ifdef __linux__
#include <termios.h>
#elif _WIN32
#include <conio.h>
#endif
#include "Window.hpp"


class Tui
{
	private:
		bool is_running;
		bool is_input;
		std::string title;
		std::queue<std::function<void()>> queue;
		std::thread *queue_thd;
		std::thread *input_thd;
		std::atomic<bool> input_lock;
		std::atomic<bool> queue_lock;
		static Tui *instance;
		Window *window;

		Tui(std::string title);
		~Tui();
		void check_queue();
		void check_input();

	public:
		void operator=(const Tui &) = delete;
		Tui(Tui &) = delete;
		static Tui * get_instance(std::string title);

		void refresh(void);
		void input_mode(std::string);
		std::array<int, 2> get_size(void);
		/* Box */
		void create_box(std::string, int, int, int, int, std::string = "");
		void delete_box(std::string);
		void draw_box(std::string);
		void move_box(std::string, int, int, int = -1, int = -1);
		void write_box(std::string, std::vector<std::string>);
		void clear_box(std::string);
		void clear_text_box(std::string);
		/* Selectable */
		void create_selec(std::string, int, int, bool, std::vector<std::string>, std::vector<std::function<void(void)>>);
		void delete_selec(std::string);
		void draw_selec(std::string);
		void clear_selec(std::string);
};

#endif
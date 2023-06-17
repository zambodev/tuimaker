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
		void input_mode(std::string mode);
		std::array<int, 2> get_size(void);
		/* Box */
		void box_create(std::string id, int x1, int y1, int x2, int y2, std::string title = "");
		void box_delete(std::string id);
		void box_draw(std::string id);
		void box_move(std::string id, int x1, int y1, int x2 = -1, int y2 = -1);
		void box_write(std::string id, std::vector<std::string> str_arr);
		void box_clear(std::string id);
		void box_clear_text(std::string id);
		/* Selectable */
		void selec_create(std::string id, int x, int y, bool is_row, std::vector<std::string> options, std::vector<std::function<void(void)>> funcs);
		void selec_delete(std::string id);
		void selec_input(std::string id);
		void selec_draw(std::string id);
		void selec_clear(std::string id);
};

#endif
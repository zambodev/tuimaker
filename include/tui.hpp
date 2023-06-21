#ifndef TUI_HPP
#define TUI_HPP

#include <fstream>
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

/** \class Tui
 *	\brief Wrapper class for auxiliary threads
*/
class Tui
{
	private:
		bool is_running;
		bool is_input;
		std::string title, stream;
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
		/** \fn static Tui * get_instance(std::string title)
		 * Get a unique instance of the class
		 * \param title Title of the Program
		*/
		static Tui * get_instance(std::string title);
		/** \fn void refresh(void)
		 * Write the buffer to screen
		*/
		void refresh(void);
		/** \fn void input_mode(std::string mode)
		 * Change input mode for the input thread
		 * \param mode "input" for text mode, "command" for command mode
		*/
		void input_mode(std::string mode);
		/** \fn std::array<int, 2> get_size(void)
		 * Return size of the terminal {max_x, max_y}
		*/
		std::array<int, 2> get_size(void);
		/** \fn FILE * get_stream(void);
		 * Return file stream
		*/
		std::string & get_stream(void);

		/******** Box ********/
		/** \fn void box_create(std::string id, int x1, int y1, int x2, int y2, std::string title = "")
		 * Create a new box instance
		 * \param id Id of the box instance
		 * \param x1 x value of the leftmost point
		 * \param y1 y value of the leftmost point
		 * \param x2 x value of the rightmost point
		 * \param y2 y value of the rightmost point
		*/
		void box_create(std::string id, int x1, int y1, int x2, int y2, std::string title = "");
		/** \fn void box_delete(std::string id)
		 * Delete box istance
		 * \param id Id of the box instance
		*/
		void box_delete(std::string id);
		/** \fn void box_draw(std::string id)
		 * Draw box into the buffer
		 * \param id Id of the box instance
		*/
		void box_draw(std::string id);
		/** \fn void box_move(std::string id, int x1, int y1, int x2 = -1, int y2 = -1)
		 * Change box coordinates and size
		 * \param id Id of the box instance
		 * \param x1 x value of the leftmost point
		 * \param y1 y value of the leftmost point
		 * \param x2 x value of the rightmost point (set only to change ratio)
		 * \param y2 y value of the rightmost point (set only to change ratio)
		*/
		void box_move(std::string id, int x1, int y1, int x2 = -1, int y2 = -1);
		/** \fn void box_write(std::string id, std::vector<std::string> str_arr)
		 * Write text into the box
		 * \param id Id of the box instance
		 * \param str_arr Array of strings
		*/
		void box_write(std::string id, std::vector<std::string> str_arr);
		/** \fn void box_clear(std::string id)
		 * Wipe box from buffer and restore the background 
		 * \param id Id of the box instance
		*/
		void box_clear(std::string id);
		/** \fn void box_clear_text(std::string id)
		 * Wipe the inside part of the box from the buffer 
		 * \param id Id of the box instance
		*/
		void box_clear_text(std::string id);
		/******** Selectable ********/
		/** \fn void selec_create(std::string id, int x, int y, bool is_row, std::vector<std::string> options, std::vector<std::function<void(void)>> funcs)
		 * Create a new selectable instance
		 * \param id Id of the selectable instance
		 * \param x x value of the leftmost point of the list
		 * \param y y value of the leftmost point of the list
		 * \param is_row Selectable rotation, true if is a row, false if is a clumn
		 * \param options List of option names
		 * \param funcs List of functions associated to the options
		*/
		void selec_create(std::string id, int x, int y, bool is_row, std::vector<std::string> options, std::vector<std::function<void(void)>> funcs);
		/** \fn void selec_delete(std::string id)
		 * Delete selectable istance
		 * \param id Id of the selectable instance
		*/
		void selec_delete(std::string id);
		/** \fn void selec_input(std::string id)
		 * Get user input for the desired selectable
		 * It uses the auxiliary thread, not the input thread 
		 * \param id Id of the selectable instance
		*/
		void selec_input(std::string id);
		/** \fn selec_draw(std::string id)
		 * Draw selectable into the buffer
		 * \param id Id of the selectable instance
		*/
		void selec_draw(std::string id);
		/** \fn void selec_clear(std::string id)
		 * Wipe selectable from the buffer
		 * \param id Id of the selectable instance
		*/
		void selec_clear(std::string id);
};

#endif
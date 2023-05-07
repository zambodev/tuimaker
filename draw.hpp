#ifndef DRAW_HPP
#define DRAW_HPP

#include <iostream>
#ifdef __linux__
#include <sys/ioctl.h>
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#endif

class Window
{
	private:
		int cols, rows;
		int input_pos[2] = {-1, -1};
		wchar_t *buffer;

		enum : wchar_t
		{
			hbar 	= u'\u2501',
			vbar 	= u'\u2503',
			tlcrn 	= u'\u250f',
			trcrn 	= u'\u2513',
			blcrn 	= u'\u2517',
			brcrn 	= u'\u251b',
			cint  	= u'\u254b',
			lint 	= u'\u2523',
			rint 	= u'\u252b',
			tint 	= u'\u2533',
			bint 	= u'\u253B'
		};

	public:
		Window(std::string = "");
		void refresh(void);
		void box(int, int, int, int, int type = 0, std::string = "");
		void hrzline(int, int, int, int);
};


#endif

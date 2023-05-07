#ifndef DRAW_HPP
#define DRAW_HPP

#include <iostream>
#include <vector>
#ifdef __linux__
#include <sys/ioctl.h>
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#endif


class Window
{
	private:
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

		class Box {
			private:
				int x1, y1, x2, y2;
				std::string title, text;

			public:
				Box(int, int, int, int, std::string = "", std::string = "");
				~Box();
				void draw(void);
				void write(std::string);
				void clear_text(void);
				void clear(void);
		};

		std::vector<Box *> boxes;
		inline static int cols = -1, rows = -1;
		inline static wchar_t *buffer = nullptr;

	public:
		Window(std::string = "");
		void refresh(void);
};


#endif

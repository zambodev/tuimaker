#include <iostream>
#include <tuple>

#ifdef __linux__
#include <sys/ioctl.h>
#include <unistd.h>
#elif _WIN32
#iclude <windows.h>
#endif

class Window
{
	private:
		int cols, rows;
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
		void refresh();
		void box(int, int, int, int, std::string = "");
		void hline(int, int, int, int);
};

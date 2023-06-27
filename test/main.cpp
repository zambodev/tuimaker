#include <iostream>
#include <thread>
#include <array>
#include <tui.hpp>


int main()
{
	Tui *tui;
	tui = tui->get_instance("Test");
	
	bool show = false;
	bool running = true;

	tui->box_create("notification", tui->get_size()[0] - 30, tui->get_size()[1] - 20, tui->get_size()[0], tui->get_size()[1], "Notifications");

	std::function<void()> notify = [tui, &show](){
		if(!show)
		{
			tui->box_draw("notification");
			tui->box_write("notification", {"Nothing to see"});
			show = true;
		}
		else
		{
			tui->box_clear("notification");
			show = false;
		}

		tui->refresh();
	};
	std::function<void()> exit = [tui, &running](){
		running = false;
	};

	tui->input_mode("command");
	tui->selec_create("Test", 2, tui->get_size()[1] - 1, true, {"Notification", "Exit"}, {notify, exit});
	tui->selec_draw("Test");
	tui->refresh();

	std::string val;

	while(running)
	{
		if(!(val = tui->get_input()).empty())
			std::wcout << val.c_str();
	}
}

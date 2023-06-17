#include <iostream>
#include <thread>
#include <array>
#include <tui.hpp>


void notif(Tui *tui, int *show)
{

}

int main()
{
	Tui *tui;
	tui = tui->get_instance("Test");
	
	int show = 0;

	tui->box_create("notification", tui->get_size()[0] - 30, tui->get_size()[1] - 20, tui->get_size()[0], tui->get_size()[1], "Notifications");

	std::function<void()> notify = [tui, &show](){
		if(show == 0)
		{
			tui->box_draw("notification");
			tui->box_write("notification", {"Nothing to see"});
			show = 1;
		}
		else
		{
			tui->box_clear("notification");
			show = 0;
		}

		tui->refresh();
	};

	tui->input_mode("command");
	tui->selec_create("Test", 2, tui->get_size()[1] - 1, true, {"Notification"}, {notify});
	tui->selec_draw("Test");
	tui->refresh();

	while(1);
}

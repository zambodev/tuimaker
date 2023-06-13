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
	tui = tui->get_instance("Test App");
	
	int show = 0;

	tui->create_box("notification", tui->get_size()[0] - 30, tui->get_size()[1] - 20, tui->get_size()[0], tui->get_size()[1], "Notifications");

	std::function<void()> notify = [tui, &show](){
		if(show == 0)
		{
			tui->draw_box("notification");
			tui->write_box("notification", {"Nothing to see"});
			show = 1;
		}
		else
		{
			tui->clear_box("notification");
			show = 0;
		}

		tui->refresh();
	};

	tui->create_selec("options", 2, tui->get_size()[1] - 1, 0, {"Notification"}, {notify});
	tui->draw_selec("options");
	tui->refresh();

	tui->input_selec("options");

	while(1);
}

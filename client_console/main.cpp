#include <fcntl.h>
#include <iostream>

#include "../daemon/config.h"

using namespace std;

int main(int argc, char* argv[]) {
	if(argc != 2)
		return -1;

	Config conf;
	auto opt = conf.GetConfig();
	int fd = open(opt.fpipe.c_str(), O_WRONLY);
	Action action = static_cast<Action>(-1);
	switch(*argv[1]) {
		case 'p': 
			action = Action::PAUSE;
			break;
		case 'q':
			action = Action::QUIT;
			break;
		case 'b':
			action = Action::PREVIOUS;
			break;
		case 'n':
			action = Action::NEXT;
			break;
		case 'r':
			action = Action::RESTART;
			break;
		default:
			cout << "Uknown action" << endl;
	}
	write(fd, &action, 1);
	close(fd);
}

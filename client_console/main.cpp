#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <string>

#include "../daemon/config.h"

using namespace std;

int main(int argc, char* argv[]) {
	if(argc != 2)
		return -1;

	Config conf;
	string s;
	auto opt = conf.GetConfig();

	ofstream file;
	file.rdbuf()->pubsetbuf(0, 0); //no buffering please
	file.open(opt.daemonpipe);
	
	fstream daemon(opt.clientpipe);

	if(!daemon.is_open() || !file.is_open())
		return 1;

	switch(*argv[1]) {
		case 'p': 
			file.put(static_cast<char>(Action::PAUSE));
			break;
		case 'q':
			file.put(static_cast<char>(Action::QUIT));
			break;
		case 'b':
			file.put(static_cast<char>(Action::PREVIOUS));
			break;
		case 'n':
			file.put(static_cast<char>(Action::NEXT));
			break;
		case 'r':
			file.put(static_cast<char>(Action::RESTART));
			break;
		case 'g':
			//I hate this
			switch(*(argv[1]+1)) {
				case 'a':
					file.put(static_cast<char>(Action::GET_ARTIST));
					getline(daemon, s);
					cout << s << endl;
					break;
				case 't':
					file.put(static_cast<char>(Action::GET_TITLE));
					getline(daemon, s);
					cout << s << endl;
					break;
			}
			break;
		default:
			cout << "Uknown action" << endl;
	}
	daemon.close();
	file.close();
}

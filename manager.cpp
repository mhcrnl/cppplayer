#include "manager.h"

#include <sys/stat.h>
#include <fstream>

//Public functions

Manager::Manager() {
	//Delete pipes, if exist (the program exit abnormaly)
	unlink(conf.GetDaemonPipe().c_str()); 
	unlink(conf.GetClientPipe().c_str());

	mkfifo(conf.GetDaemonPipe().c_str(), 0666);
	mkfifo(conf.GetClientPipe().c_str(), 0666);
}

Manager::~Manager() {
	unlink(conf.GetDaemonPipe().c_str()); 
	unlink(conf.GetClientPipe().c_str());
}

void Manager::StartServer() {
	while(music.GetStatus() != Status::Exit) {
		auto command = ReadCommand();
	}
}


//Private Functions 

Command Manager::ReadCommand() {
	std::ifstream f(conf.GetDaemonPipe());
	if(!f.is_open()) {
		throw std::runtime_error("Daemon pipe culd not be opened");
	}

	auto tmp = static_cast<Command>(f.get());
	f.close();
	return tmp;
}
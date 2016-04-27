#include "manager.h"
#include "musiclist.h"

#include <sys/stat.h>
#include <fstream>
#include <thread>

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
	music.GetList().LoadDir(conf.GetDir());
	std::thread mplayer( [this] { music.PlayList(); } );

	if(conf.GetAutostart()) {
		music.SetStatus(Status::Playing);
	}

	while(music.GetStatus() != Status::Exit) {
		ExecuteCommand(ReadCommand());
	}
	mplayer.join();
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

void Manager::ExecuteCommand(Command c) {
	switch (c) {
		case Command::QUIT:
			music.SetStatus(Status::Exit);
			break;
	}
}
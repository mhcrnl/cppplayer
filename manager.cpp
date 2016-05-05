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
		case Command::PLAY:
			music.SetStatus(Status::Playing);
			break;
		case Command::PAUSE:
			if(music.GetStatus() != Status::Playing)
				music.SetStatus(Status::Playing);
			else
				music.SetStatus(Status::Paused);
			break;
		case Command::STOP:
			music.SetStatus(Status::Stoped);
			break;
		case Command::NEXT:
			music.SetStatus(Status::Forwarding);
			break;
		case Command::BACK:
			music.SetStatus(Status::Backing);
			break;
		case Command::SORT_RANDOM:
			music.GetList().Sort(Order::RANDOM);
			break;
		case Command::GET_ARTIST:
			{
				std::fstream file(conf.GetClientPipe());
				file << music.GetCurrent().GetArtist() << std::endl;
				file.close();
			}
			break;
		case Command::GET_TITLE:
			{
				std::fstream file(conf.GetClientPipe());
				file << music.GetCurrent().GetTitle() << std::endl;
				file.close();
			}
			break;
		case Command::GET_FILE:
			{
				std::fstream file(conf.GetClientPipe());
				file << music.GetCurrent().GetFile() << std::endl;
				file.close();
			}
			break;
	}
}